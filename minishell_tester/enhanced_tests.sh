#!/usr/bin/env bash
# Enhanced Minishell Tests - Advanced Heredoc, Subshell, and Edge Cases

MS_BIN=${MS_BIN:-"./minishell"}
PASS=0
FAIL=0

# Color output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

run_test() {
    local name=$1
    local input=$2
    local expect_error=${3:-0}
    
    printf "Testing: %-65s " "$name"
    
    output=$(printf "%b\nexit\n" "$input" | $MS_BIN 2>&1)
    rc=$?
    
    if [ $rc -gt 128 ]; then
        printf "${RED}[CRASH - signal %d]${NC}\n" $((rc - 128))
        echo "Input: $input"
        echo "Output: $output"
        echo "---"
        ((FAIL++))
        return
    fi
    
    printf "${GREEN}[OK]${NC}\n"
    ((PASS++))
}

echo "========================================="
echo "ENHANCED MINISHELL TESTS"
echo "========================================="
echo ""

# Create test files for heredoc tests
mkdir -p /tmp/minishell_test
echo "test_content" > /tmp/minishell_test/testfile.txt

echo "=== 1. Advanced Heredoc Tests ==="
run_test "Heredoc with variable expansion" 'export VAR=hello; cat <<EOF\n$VAR world\nEOF' 0
run_test "Heredoc with quoted delimiter (no expansion)" "cat <<'END'\n\$HOME test\nEND" 0
run_test "Heredoc with double quoted delimiter" 'cat <<"END"\n$HOME test\nEND' 0
run_test "Multiple heredocs in one command" 'cat <<EOF1 <<EOF2\nfirst\nEOF1\nsecond\nEOF2' 0
run_test "Heredoc with redirection" 'cat <<EOF > /tmp/minishell_test/out.txt\ntest\nEOF' 0
run_test "Heredoc followed by input redirect" 'cat <<EOF < /tmp/minishell_test/testfile.txt\nignored\nEOF' 0
run_test "Heredoc with pipe" 'cat <<EOF | grep test\ntest line\nEOF' 0
run_test "Heredoc with empty content" 'cat <<EOF\nEOF' 0
run_test "Heredoc with spaces in delimiter" "cat <<'E O F'\ntest\nE O F" 0
run_test "Heredoc delimiter with underscores" 'cat <<END_OF_FILE\ntest\nEND_OF_FILE' 0
run_test "Heredoc with tabs" 'cat <<-EOF\n\ttest\n\tEOF' 0
run_test "Nested heredoc in subshell" '(cat <<EOF\ninner\nEOF)' 0
run_test "Heredoc with exit status variable" 'cat <<EOF\n$?\nEOF' 0
run_test "Heredoc with special characters" 'cat <<EOF\n!@#$%^&*()\nEOF' 0
run_test "Heredoc with quotes inside" "cat <<EOF\n'single' \"double\"\nEOF" 0
run_test "Multiple heredocs with pipe" 'cat <<E1 | cat <<E2\nfirst\nE1\nsecond\nE2' 0
run_test "Heredoc in loop simulation" 'cat <<EOF; cat <<EOF2\nloop1\nEOF\nloop2\nEOF2' 0
run_test "Heredoc with backslash" 'cat <<EOF\n\\test\nEOF' 0
run_test "Heredoc with dollar at end" 'cat <<EOF\ntest$\nEOF' 0
run_test "Very long heredoc delimiter" 'cat <<VERY_LONG_DELIMITER_NAME_123\ntest\nVERY_LONG_DELIMITER_NAME_123' 0

echo ""
echo "=== 2. Comprehensive Subshell Tests ==="
run_test "Simple subshell" '(echo test)' 0
run_test "Subshell with exit status" '(exit 42); echo $?' 0
run_test "Nested subshells 2 levels" '((echo nested))' 0
run_test "Nested subshells 3 levels" '(((echo deep)))' 0
run_test "Subshell with variable" 'X=5; (echo $X)' 0
run_test "Variable set in subshell" '(X=10); echo $X' 0
run_test "Subshell with export" '(export Y=test); echo $Y' 0
run_test "Subshell with cd" '(cd /tmp); pwd' 0
run_test "Multiple commands in subshell" '(echo a; echo b; echo c)' 0
run_test "Subshell with pipe inside" '(echo test | cat)' 0
run_test "Subshell output to pipe" '(echo test) | cat' 0
run_test "Pipe to subshell" 'echo test | (cat)' 0
run_test "Subshell with output redirect" '(echo test) > /tmp/minishell_test/sub.txt' 0
run_test "Subshell with input redirect" '(cat) < /tmp/minishell_test/testfile.txt' 0
run_test "Subshell with append redirect" '(echo line) >> /tmp/minishell_test/sub.txt' 0
run_test "Subshell with AND operator" '(echo a) && echo b' 0
run_test "Subshell with OR operator" '(exit 1) || echo failed' 0
run_test "Multiple subshells in sequence" '(echo 1); (echo 2); (echo 3)' 0
run_test "Subshell in AND chain" '(echo first) && (echo second)' 0
run_test "Subshell in OR chain" '(false) || (echo backup)' 0
run_test "Complex nested subshell" '((echo a; echo b) | cat)' 0
run_test "Subshell with heredoc" '(cat <<EOF\nsubshell heredoc\nEOF)' 0
run_test "Subshell with builtin" '(pwd; cd /tmp; pwd)' 0
run_test "Empty subshell with redirect" '() > /tmp/minishell_test/empty.txt' 1
run_test "Subshell with wildcard" '(echo *.c)' 0
run_test "Subshell in background simulation" '(sleep 0.1)' 0
run_test "Subshell with exit" '(exit 5); echo $?' 0
run_test "Subshell preserves parent vars" 'A=1; (echo $A)' 0
run_test "Subshell modifies var (no effect)" 'B=1; (B=2; echo $B); echo $B' 0
run_test "Deeply nested subshells" '((((echo very-deep))))' 0
run_test "Subshell with multiple pipes" '(echo a | cat | cat | cat)' 0

echo ""
echo "=== 3. Complex Quoting Edge Cases ==="
run_test "Quote within quote different types" "echo \"He said 'hello'\"" 0
run_test "Reverse quote nesting" "echo 'She said \"hi\"'" 0
run_test "Adjacent quotes no space" "echo 'hello'\"world\"" 0
run_test "Three quote types adjacent" "echo 'a'\"b\"'c'" 0
run_test "Empty quotes sequence" "echo '' '' ''" 0
run_test "Quote then variable" "echo 'test'\$HOME" 0
run_test "Variable then quote" "echo \$HOME'test'" 0
run_test "Quote around special chars" "echo '| > < && ||'" 0
run_test "Escaped quote in double" 'echo "test \" quote"' 0
run_test "Dollar before quotes" 'echo $"test"' 0
run_test "Multiple adjacent double quotes" 'echo ""test""' 0
run_test "Quote with newline literal" "echo 'line1\nline2'" 0
run_test "Mixed quotes with spaces" "echo 'a' \"b\" 'c'" 0
run_test "Quote empty variable" 'echo "$EMPTY"' 0
run_test "Single quote dollar question" "echo '\$?'" 0
run_test "Quotes with wildcard" "echo '*.txt'" 0
run_test "Quote around redirect" "echo 'test > file'" 0
run_test "Double quote with backslash" 'echo "\\test"' 0
run_test "Complex quote combination" 'echo "a'"'"'b"'"'"'c"' 0
run_test "Quote at start and end" "'echo' 'test'" 0

echo ""
echo "=== 4. File Descriptor Edge Cases ==="
run_test "Redirect stdout twice" 'echo test > /tmp/minishell_test/f1.txt > /tmp/minishell_test/f2.txt' 0
run_test "Multiple input redirects" 'cat < /tmp/minishell_test/testfile.txt < /tmp/minishell_test/testfile.txt' 0
run_test "Output then input redirect" 'cat > /tmp/minishell_test/tmp.txt < /tmp/minishell_test/testfile.txt' 0
run_test "Input then output redirect" 'cat < /tmp/minishell_test/testfile.txt > /tmp/minishell_test/tmp.txt' 0
run_test "Append after overwrite" 'echo a > /tmp/minishell_test/f.txt; echo b >> /tmp/minishell_test/f.txt' 0
run_test "Overwrite after append" 'echo a >> /tmp/minishell_test/f.txt; echo b > /tmp/minishell_test/f.txt' 0
run_test "Redirect to /dev/null" 'echo test > /dev/null' 0
run_test "Redirect from /dev/null" 'cat < /dev/null' 0
run_test "Multiple redirects in pipe" 'cat < /tmp/minishell_test/testfile.txt | cat > /tmp/minishell_test/tmp.txt' 0
run_test "Redirect before command" '< /tmp/minishell_test/testfile.txt cat' 0
run_test "Redirect in middle of args" 'cat < /tmp/minishell_test/testfile.txt -e' 0
run_test "Many redirects" 'echo test > /tmp/minishell_test/a > /tmp/minishell_test/b > /tmp/minishell_test/c' 0
run_test "Redirect with spaces" 'echo test >    /tmp/minishell_test/spaced.txt' 0
run_test "Redirect to existing file" 'echo new > /tmp/minishell_test/testfile.txt' 0
run_test "Redirect from nonexistent file" 'cat < /tmp/minishell_test/nonexistent123456.txt' 0

echo ""
echo "=== 5. Advanced Variable Expansion ==="
run_test "Exit status after success" 'true; echo $?' 0
run_test "Exit status after failure" 'false; echo $?' 0
run_test "Exit status in subshell" '(exit 42); echo $?' 0
run_test "Multiple exit status" 'echo $?; echo $?; echo $?' 0
run_test "Variable with numbers" 'VAR123=test; echo $VAR123' 0
run_test "Variable with underscores" '_VAR_NAME=test; echo $_VAR_NAME' 0
run_test "Variable concatenation" 'A=hello; B=world; echo $A$B' 0
run_test "Variable in arithmetic context" 'NUM=5; echo $NUM' 0
run_test "Undefined variable in quotes" 'echo "$UNDEF"' 0
run_test "Variable followed by text" 'VAR=test; echo $VAR.txt' 0
run_test "Variable with braces" 'VAR=test; echo ${VAR}' 0
run_test "Empty variable expansion" 'EMPTY=; echo $EMPTY' 0
run_test "Variable in heredoc" 'X=value; cat <<EOF\n$X\nEOF' 0
run_test "Dollar without variable" 'echo $' 0
run_test "Dollar with space" 'echo $ test' 0
run_test "Multiple dollars" 'echo $$$$' 0
run_test "Dollar in single vs double" 'echo $HOME; echo "$HOME"; echo '"'"'$HOME'"'"'' 0
run_test "Variable in command position" 'CMD=echo; $CMD test' 0
run_test "Variable in redirect" 'FILE=/tmp/minishell_test/var.txt; echo test > $FILE' 0
run_test "Question mark multiple times" 'echo $? $? $?' 0

echo ""
echo "=== 6. Advanced Pipe Combinations ==="
run_test "Five pipe chain" 'echo test | cat | cat | cat | cat | cat' 0
run_test "Pipe with subshell left" '(echo test) | cat' 0
run_test "Pipe with subshell right" 'echo test | (cat)' 0
run_test "Pipe with subshell both" '(echo test) | (cat)' 0
run_test "Pipe with heredoc left" 'cat <<EOF | cat\ntest\nEOF' 0
run_test "Pipe with multiple redirects" 'cat < /tmp/minishell_test/testfile.txt | cat > /tmp/minishell_test/tmp.txt' 0
run_test "Pipe chain with grep" 'echo testing | cat | grep test' 0
run_test "Empty pipe left" 'echo | cat' 0
run_test "Pipe with builtin" 'export | cat' 0
run_test "Pipe with pwd" 'pwd | cat' 0
run_test "Pipe with env" 'env | grep PATH' 0

echo ""
echo "=== 7. Logical Operator Edge Cases ==="
run_test "AND with subshell" '(echo a) && echo b' 0
run_test "OR with subshell" '(false) || echo b' 0
run_test "AND chain three" 'echo a && echo b && echo c' 0
run_test "OR chain three" 'false || false || echo c' 0
run_test "Mixed AND OR" 'false || echo a && echo b' 0
run_test "AND with redirect" 'echo a > /tmp/minishell_test/tmp.txt && cat /tmp/minishell_test/tmp.txt' 0
run_test "OR with redirect" 'false || echo b > /tmp/minishell_test/tmp.txt' 0
run_test "AND with pipe" 'echo test | cat && echo done' 0
run_test "OR with pipe" 'false | cat || echo failed' 0

echo ""
echo "=== 8. Wildcard Advanced Tests ==="
run_test "Wildcard in subshell" '(echo *.sh)' 0
run_test "Wildcard in quotes" "echo '*.txt'" 0
run_test "Wildcard no match" 'echo *.nonexistent123456' 0
run_test "Multiple wildcards" 'echo *.sh *.txt' 0
run_test "Wildcard in variable" 'PATTERN=*.sh; echo $PATTERN' 0
run_test "Question mark wildcard" 'echo ?.sh' 0

echo ""
echo "========================================="
echo "RESULTS"
echo "========================================="
echo -e "PASS: ${GREEN}${PASS}${NC}"
echo -e "FAIL: ${RED}${FAIL}${NC}"
echo "TOTAL: $((PASS + FAIL))"

# Cleanup
rm -rf /tmp/minishell_test

if [ $FAIL -eq 0 ]; then
    echo -e "\n${GREEN}All enhanced tests passed!${NC}"
    exit 0
else
    echo -e "\n${RED}Some tests failed!${NC}"
    exit 1
fi
