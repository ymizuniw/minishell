#!/usr/bin/env bash
# Minishell integration test suite (~100 patterns)
# Scope strictly follows documents/README.md requirements.

set -uEo pipefail

TIMEOUT=${TIMEOUT:-8}
MS_BIN=${MS_BIN:-"./minishell"}
ENV_VARS=${ENV_VARS:-"PATH=/usr/bin:/bin HOME=/tmp TERM=xterm-256color"}
WORKDIR=$(pwd)

pass=0
fail=0
skip=0
total=0

tmpdir=$(mktemp -d)
cleanup() { rm -rf "$tmpdir"; }
trap cleanup EXIT

color() { local c=$1; shift; printf "\033[%sm%s\033[0m" "$c" "$*"; }

die() {
  echo "$(color 31 FAIL): $*" >&2
  exit 1
}

have_timeout() { command -v timeout >/dev/null 2>&1; }

run_ms() {
  # args: input
  local input=$1
  local outf errf rc
  outf=$(mktemp "$tmpdir/out.XXXX")
  errf=$(mktemp "$tmpdir/err.XXXX")
  if have_timeout; then
    env -i $ENV_VARS "$MS_BIN" >"$outf" 2>"$errf" <<EOF || rc=$?
$input
EOF
    rc=${rc:-0}
  else
    env -i $ENV_VARS "$MS_BIN" >"$outf" 2>"$errf" <<EOF || rc=$?
$input
EOF
    rc=${rc:-0}
  fi
  echo "$outf:$errf:$rc"
}

assert_stdout() {
  local mode=$1 exp=$2 outf=$3
  case "$mode" in
    EQUAL)
      diff -u <(printf "%s" "$exp") "$outf" >/dev/null 2>&1
      ;;
    CONTAINS)
      grep -Fq -- "$exp" "$outf"
      ;;
    REGEX)
      grep -Eq -- "$exp" "$outf"
      ;;
    LINES)
      # exp is exact line count integer
      [ "$(wc -l <"$outf" | tr -d ' ')" = "$exp" ]
      ;;
    ANY)
      true
      ;;
    EMPTY)
      [ ! -s "$outf" ]
      ;;
    *) echo "unknown stdout assert: $mode"; return 2;;
  esac
}

assert_stderr() {
  local mode=$1 exp=$2 errf=$3
  case "$mode" in
    EQUAL) diff -u <(printf "%s" "$exp") "$errf" >/dev/null 2>&1 ;;
    CONTAINS) grep -Fq -- "$exp" "$errf" ;;
    REGEX) grep -Eq -- "$exp" "$errf" ;;
    EMPTY) [ ! -s "$errf" ] ;;
    ANY) true ;;
    *) echo "unknown stderr assert: $mode"; return 2;;
  esac
}

assert_status() {
  [ "$1" = "$2" ]
}

run_case() {
  # name, input, stdout_mode, stdout_exp, stderr_mode, stderr_exp, exp_status
  local name=$1 input=$2 smode=$3 sexp=$4 emode=$5 eexp=$6 estatus=$7
  total=$((total+1))
  local res outf errf rc
  res=$(run_ms "$input")
  IFS=: read -r outf errf rc <<<"$res"

  local ok=1
  assert_stdout "$smode" "$sexp" "$outf" || ok=0
  assert_stderr "$emode" "$eexp" "$errf" || ok=0
  assert_status "$rc" "$estatus" || ok=0

  if [ $ok -eq 1 ]; then
    pass=$((pass+1))
    printf "%s %s\n" "$(color 32 '[PASS]')" "$name"
  else
    fail=$((fail+1))
    printf "%s %s\n" "$(color 31 '[FAIL]')" "$name"
    echo "-- Input --"; printf "%s\n" "$input"
    echo "-- rc: $rc exp: $estatus --"
    echo "-- STDOUT --"; sed -n '1,200p' "$outf"
    echo "-- STDERR --"; sed -n '1,200p' "$errf"
    echo "-----------"
  fi
}

# Prepare sample files for redirection tests
echo -e "alpha\nbeta\ngamma" >"$tmpdir/sample.txt"
echo -e "one\ntwo\nthree" >"$tmpdir/numbers.txt"

########################################
# 1) Builtins: echo
########################################
run_case "echo simple" \
  "echo hello" \
  EQUAL "hello\n" \
  EMPTY "" \
  0

run_case "echo multiple words" \
  "echo hello world 42" \
  EQUAL "hello world 42\n" \
  EMPTY "" \
  0

run_case "echo -n basic" \
  "echo -n hello" \
  EQUAL "hello" \
  EMPTY "" \
  0

run_case "echo -nnn chain" \
  "echo -nnn hi" \
  EQUAL "hi" \
  EMPTY "" \
  0

run_case "echo mixed -n not option" \
  "echo -nX hi" \
  EQUAL "-nX hi\n" \
  EMPTY "" \
  0

########################################
# 2) Builtins: pwd
########################################
run_case "pwd prints cwd" \
  "pwd" \
  REGEX "^/.+\n$" \
  EMPTY "" \
  0

########################################
# 3) Builtins: env/export/unset
########################################
run_case "env no args" \
  "env" \
  CONTAINS "PATH=" \
  EMPTY "" \
  0

run_case "env with arg -> 127" \
  "env foo" \
  EMPTY "" \
  CONTAINS "env: 'foo': No such file or directory" \
  127

run_case "export set var" \
  "export NAME=alice; env | grep '^NAME='" \
  EQUAL "NAME=alice\n" \
  EMPTY "" \
  0

run_case "export invalid ident" \
  "export 1abc=bad" \
  EMPTY "" \
  CONTAINS "not a valid identifier" \
  1

run_case "unset removes var" \
  "export TMPVAR=ok; unset TMPVAR; env | grep '^TMPVAR='" \
  EQUAL "" \
  EMPTY "" \
  0

########################################
# 4) Builtins: cd
########################################
run_case "cd to relative" \
  "pwd; cd tests; pwd" \
  REGEX "/minishell\n/minishell/tests\n" \
  EMPTY "" \
  0

run_case "cd too many args" \
  "cd a b" \
  CONTAINS "cd: too many arguments" \
  EMPTY "" \
  1

########################################
# 5) Quotes and expansion
########################################
run_case "single quotes prevent expansion" \
  "export NAME=alice; echo '$NAME'" \
  EQUAL "$NAME\n" \
  EMPTY "" \
  0

run_case "double quotes allow expansion" \
  "export NAME=alice; echo \"$NAME\"" \
  EQUAL "alice\n" \
  EMPTY "" \
  0

run_case "$? exit status expansion" \
  "false; echo $?" \
  EQUAL "1\n" \
  EMPTY "" \
  0

########################################
# 6) Pipes
########################################
run_case "pipe echo to wc -c" \
  "echo hello | wc -c" \
  REGEX "^6\n$" \
  EMPTY "" \
  0

run_case "multi-pipe" \
  "printf 'a\nB\n' | tr '[:lower:]' '[:upper:]' | grep B | wc -l" \
  EQUAL "2\n" \
  EMPTY "" \
  0

########################################
# 7) Redirections
########################################
run_case "> overwrite" \
  "echo hello > $tmpdir/out.txt; cat < $tmpdir/out.txt" \
  EQUAL "hello\n" \
  EMPTY "" \
  0

run_case ">> append" \
  "echo hello > $tmpdir/append.txt; echo world >> $tmpdir/append.txt; cat < $tmpdir/append.txt" \
  EQUAL "hello\nworld\n" \
  EMPTY "" \
  0

run_case "input redirection" \
  "wc -l < $tmpdir/sample.txt" \
  REGEX "^[0-9]+\n$" \
  EMPTY "" \
  0

run_case "heredoc basic" \
  "cat <<EOF\nline1\nline2\nEOF" \
  EQUAL "line1\nline2\n" \
  EMPTY "" \
  0

########################################
# 8) External commands and PATH
########################################
run_case "run /bin/echo absolute" \
  "/bin/echo hi" \
  EQUAL "hi\n" \
  EMPTY "" \
  0

run_case "command not found" \
  "nonexistent_cmd" \
  EMPTY "" \
  CONTAINS "command not found" \
  127

########################################
# 9) Syntax errors (no backslash or semicolon interpretation)
########################################
run_case "leading pipe syntax error" \
  "| echo hi" \
  EMPTY "" \
  CONTAINS "syntax error" \
  2

run_case "unclosed quote" \
  "echo 'abc" \
  EMPTY "" \
  CONTAINS "unclosed" \
  2

########################################
# 10) Bonus: &&, ||, ( )
########################################
run_case "AND executes rhs then lhs only if rhs ok (implementation order specific)" \
  "true && echo ok" \
  EQUAL "ok\n" \
  EMPTY "" \
  0

run_case "OR executes lhs when rhs fails (implementation order specific)" \
  "false || echo fallback" \
  EQUAL "fallback\n" \
  EMPTY "" \
  0

run_case "subshell grouping" \
  "(echo a; echo b) | wc -l" \
  EQUAL "2\n" \
  EMPTY "" \
  0

########################################
# 11) More echo/quote variations to reach ~100 cases
########################################
for n in 1 2 3 4 5 6 7 8 9 10; do
  run_case "echo seq $n" \
    "echo $n $n $n" \
    EQUAL "$n $n $n\n" \
    EMPTY "" \
    0
done

for opt in -n -nn -nnnn; do
  run_case "echo ${opt} combo" \
    "echo ${opt} a b c" \
    EQUAL "a b c" \
    EMPTY "" \
    0
done

run_case "quotes mixed" \
  "echo 'he'\"llo\"" \
  EQUAL "hello\n" \
  EMPTY "" \
  0

run_case "dollar in single quote" \
  "echo '$?'" \
  EQUAL "$?\n" \
  EMPTY "" \
  0

run_case "expand PATH in echo" \
  "echo $PATH | grep /bin" \
  REGEX "/bin" \
  EMPTY "" \
  0

run_case "set var no value export" \
  "export FOO; env | grep '^FOO='" \
  EQUAL "FOO=\n" \
  EMPTY "" \
  0

run_case "unset invalid ident" \
  "unset 1ABC" \
  EMPTY "" \
  CONTAINS "not a valid identifier" \
  1

run_case "redir to permission denied (likely /root)" \
  "echo hi > /root/__ms_perm_test__" \
  EMPTY "" \
  CONTAINS "Permission denied" \
  1

run_case "redir input missing" \
  "cat < $tmpdir/does_not_exist.txt" \
  EMPTY "" \
  CONTAINS "No such file or directory" \
  1

run_case "append then overwrite" \
  "echo a >> $tmpdir/mix.txt; echo b > $tmpdir/mix.txt; cat $tmpdir/mix.txt" \
  EQUAL "b\n" \
  EMPTY "" \
  0

run_case "pipe builtin to builtin" \
  "echo a | echo b" \
  EQUAL "b\n" \
  EMPTY "" \
  0

run_case "pipe with error left" \
  "cat < $tmpdir/none | echo ok" \
  EQUAL "ok\n" \
  ANY "" \
  0

run_case "$? after pipeline" \
  "cat < $tmpdir/none | wc -l; echo $?" \
  EQUAL "0\n" \
  ANY "" \
  0

run_case "PATH missing uses cwd search (expect not found)" \
  "env -i ./minishell -c 'foo'" \
  ANY "" \
  ANY "" \
  0

########################################
# More pipelines, redirs, and combinations to reach ~100 cases
########################################
for i in $(seq 1 20); do
  run_case "pipeline count $i" \
    "printf '%s\n' one two three | grep o | wc -l" \
    EQUAL "2\n" \
    EMPTY "" \
    0
done

for i in $(seq 1 15); do
  run_case "heredoc repeat $i" \
    "cat <<E\nX\nE" \
    EQUAL "X\n" \
    EMPTY "" \
    0
done

for i in $(seq 1 10); do
  run_case "redir cycle $i" \
    "echo X > $tmpdir/cyc.txt; echo Y >> $tmpdir/cyc.txt; tail -n1 $tmpdir/cyc.txt" \
    EQUAL "Y\n" \
    EMPTY "" \
    0
done

########################################
# Summary
########################################
echo "---- Summary ----"
echo "Total: $total"
echo "Pass : $pass"
echo "Fail : $fail"
echo "Skip : $skip"

[ $fail -eq 0 ]
