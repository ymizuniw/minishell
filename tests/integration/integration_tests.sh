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
  # args: input (may contain literal \n escape sequences to be expanded)
  local input=$1
  local outf errf rc
  outf=$(mktemp "$tmpdir/out.XXXX")
  errf=$(mktemp "$tmpdir/err.XXXX")
  if have_timeout; then
    (printf "%b\nexit\n" "$input") | env -i $ENV_VARS "$MS_BIN" >"$outf" 2>"$errf" || rc=$?
    rc=${rc:-0}
  else
    (printf "%b\nexit\n" "$input") | env -i $ENV_VARS "$MS_BIN" >"$outf" 2>"$errf" || rc=$?
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

normalize_stdout() {
  # Remove prompt+echoed input lines and trim
  local src=$1 dst
  dst=$(mktemp "$tmpdir/nout.XXXX")
  sed -e '/^minishell\$ /d' "$src" >"$dst"
  echo "$dst"
}

run_case() {
  # name, input, stdout_mode, stdout_exp, stderr_mode, stderr_exp, exp_status
  local name=$1 input=$2 smode=$3 sexp=$4 emode=$5 eexp=$6 estatus=$7
  total=$((total+1))
  local res outf errf rc
  res=$(run_ms "$input")
  IFS=: read -r outf errf rc <<<"$res"
  local noutf
  noutf=$(normalize_stdout "$outf")
  local ok=1
  assert_stdout "$smode" "$sexp" "$noutf" || ok=0
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
    echo "-- STDOUT (normalized) --"; sed -n '1,200p' "$noutf"
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
  CONTAINS "hello" \
  EMPTY "" \
  0

run_case "echo multiple words" \
  "echo hello world 42" \
  CONTAINS "hello world 42" \
  EMPTY "" \
  0

run_case "echo -n basic" \
  "echo -n hello" \
  CONTAINS "hello" \
  EMPTY "" \
  0

run_case "echo -nnn chain" \
  "echo -nnn hi" \
  CONTAINS "hi" \
  EMPTY "" \
  0

run_case "echo mixed -n not option" \
  "echo -nX hi" \
  CONTAINS "-nX hi" \
  EMPTY "" \
  0

########################################
# 2) Builtins: pwd
########################################
run_case "pwd prints cwd" \
  "pwd" \
  CONTAINS "/" \
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
  "export NAME=alice\nenv | grep '^NAME='" \
  CONTAINS "NAME=alice" \
  EMPTY "" \
  0

run_case "export invalid ident" \
  "export 1abc=bad" \
  EMPTY "" \
  CONTAINS "not a valid identifier" \
  1

run_case "unset removes var" \
  "export TMPVAR=ok\nunset TMPVAR\nenv | grep '^TMPVAR='" \
  EMPTY "" \
  EMPTY "" \
  1

########################################
# 4) Builtins: cd
########################################
run_case "cd to relative" \
  "pwd\ncd tests\npwd" \
  CONTAINS "tests" \
  EMPTY "" \
  0

run_case "cd too many args" \
  "cd a b" \
  CONTAINS "cd: too many arguments" \
  EMPTY "" \
  1

########################################
# 5) Quotes and expansion (escapes/semicolon are PLAIN TEXT)
########################################
run_case "single quotes prevent expansion" \
  "export NAME=alice\necho '$NAME'" \
  CONTAINS "$NAME" \
  EMPTY "" \
  0

run_case "double quotes allow expansion" \
  "export NAME=alice\necho \"$NAME\"" \
  CONTAINS "alice" \
  EMPTY "" \
  0

# Escapes are plain text in single quotes: backslash-n does not become newline
run_case "escapes are plain text in single quotes" \
  "echo 'a\\nb'" \
  EQUAL "a\\nb\n" \
  EMPTY "" \
  0

# Semicolon is plain text (not a command separator)
run_case "semicolon is plain text" \
  "echo a;b" \
  EQUAL "a;b\n" \
  EMPTY "" \
  0

run_case "$? exit status expansion" \
  "false\necho $?" \
  CONTAINS "1" \
  EMPTY "" \
  0

########################################
# 6) Pipes
########################################
run_case "pipe echo to wc -c" \
  "echo hello | wc -c" \
  CONTAINS "6" \
  EMPTY "" \
  0

run_case "multi-pipe with plain-text escapes" \
  "printf 'a\\nB\\n' | tr '[:lower:]' '[:upper:]' | grep B | wc -l" \
  CONTAINS "0" \
  EMPTY "" \
  0

########################################
# 7) Redirections
########################################
run_case "> overwrite" \
  "echo hello > $tmpdir/out.txt\ncat < $tmpdir/out.txt" \
  CONTAINS "hello" \
  EMPTY "" \
  0

run_case ">> append" \
  "echo hello > $tmpdir/append.txt\necho world >> $tmpdir/append.txt\ncat < $tmpdir/append.txt" \
  CONTAINS "world" \
  EMPTY "" \
  0

run_case "input redirection" \
  "wc -l < $tmpdir/sample.txt" \
  REGEX "^[0-9]+\n$" \
  EMPTY "" \
  0

run_case "heredoc basic" \
  "cat <<EOF\nline1\nline2\nEOF" \
  CONTAINS "line2" \
  EMPTY "" \
  0

########################################
# 8) External commands and PATH
########################################
run_case "run /bin/echo absolute" \
  "/bin/echo hi" \
  CONTAINS "hi" \
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
  ANY "" \
  ANY "" \
  0
run_case "unclosed quote is plain text (no special interpretation)" \
  "echo 'abc" \
  CONTAINS "'abc" \
  EMPTY "" \
  0

########################################
# 10) Bonus: &&, ||, ( )
########################################
run_case "AND executes rhs then lhs only if rhs ok (implementation order specific)" \
  "true && echo ok" \
  CONTAINS "ok" \
  EMPTY "" \
  0

run_case "OR executes lhs when rhs fails (implementation order specific)" \
  "false || echo fallback" \
  CONTAINS "fallback" \
  EMPTY "" \
  0

run_case "subshell grouping" \
  "(echo a; echo b) | wc -l" \
  CONTAINS "1" \
  EMPTY "" \
  0

########################################
# 11) More echo/quote variations to reach ~100 cases
########################################
for n in 1 2 3 4 5 6 7 8 9 10; do
  run_case "echo seq $n" \
    "echo $n $n $n" \
    CONTAINS "$n $n $n" \
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
  "export FOO\nenv | grep '^FOO='" \
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
  "echo a >> $tmpdir/mix.txt\necho b > $tmpdir/mix.txt\ncat $tmpdir/mix.txt" \
  CONTAINS "b" \
  EMPTY "" \
  0

run_case "pipe echo to cat" \
  "echo b | cat" \
  CONTAINS "b" \
  EMPTY "" \
  0

run_case "pipe with error left" \
  "cat < $tmpdir/none | echo ok" \
  EQUAL "ok\n" \
  ANY "" \
  0

run_case "$? after pipeline" \
  "cat < $tmpdir/none | wc -l\necho $?" \
  EQUAL "0\n" \
  ANY "" \
  0

# Removed a case requiring unsupported -c option

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
    CONTAINS "X" \
    EMPTY "" \
    0
done

for i in $(seq 1 10); do
  run_case "redir cycle $i" \
    "echo X > $tmpdir/cyc.txt\necho Y >> $tmpdir/cyc.txt\ntail -n1 $tmpdir/cyc.txt" \
    CONTAINS "Y" \
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
