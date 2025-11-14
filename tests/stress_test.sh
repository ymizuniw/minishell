#!/usr/bin/env bash
# Minishell Stress Test - 徹底的な破壊テスト

MS_BIN=${MS_BIN:-"./minishell"}
PASS=0
FAIL=0

# Color output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

run_test() {
    local name=$1
    local input=$2
    local expect_error=${3:-0}  # 1 if we expect error, 0 if not
    
    printf "Testing: %-60s " "$name"
    
    # Run the test
    output=$(printf "%b\nexit\n" "$input" | $MS_BIN 2>&1)
    rc=$?
    
    # Check if it crashed (segfault, etc)
    if [ $rc -gt 128 ]; then
        printf "${RED}[CRASH - signal %d]${NC}\n" $((rc - 128))
        echo "Input: $input"
        echo "Output: $output"
        echo "---"
        ((FAIL++))
        return
    fi
    
    # For error cases, just check it didn't crash
    if [ $expect_error -eq 1 ]; then
        printf "${GREEN}[OK]${NC}\n"
        ((PASS++))
    else
        printf "${GREEN}[OK]${NC}\n"
        ((PASS++))
    fi
}

echo "========================================="
echo "MINISHELL STRESS TEST"
echo "========================================="
echo ""

echo "=== 1. Quote Edge Cases ==="
run_test "Empty quotes" "echo ''" 0
run_test "Nested quote attempts" "echo 'test\"inner\"test'" 0
run_test "Multiple unclosed quotes" "echo 'a 'b 'c" 1
run_test "Quote at end of input" "echo test'" 1
run_test "Only quote character" "'" 1
run_test "Multiple quotes only" "'''" 1
run_test "Alternating quotes" "echo '\"'\"'\"" 0
run_test "Quote in word middle" "ec'h'o test" 0
run_test "Empty double quotes" 'echo ""' 0
run_test "Quote after pipe" "echo test | '" 1

echo ""
echo "=== 2. Variable Expansion Edge Cases ==="
run_test "Undefined variable" 'echo $UNDEFINED_VAR' 0
run_test "Multiple dollars" 'echo $$$$' 0
run_test "Dollar at end" 'echo test$' 0
run_test "Dollar with special chars" 'echo $@#%' 0
run_test "Very long variable name" 'echo $AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA' 0
run_test "Dollar in single quotes" "echo '\$USER'" 0
run_test "Empty variable name" 'echo ${}' 0
run_test "Dollar with numbers" 'echo $123' 0
run_test "Dollar with underscore" 'echo $_TEST_VAR' 0
run_test "Consecutive dollars" 'echo $USER$HOME$PATH' 0

echo ""
echo "=== 3. Wildcard Edge Cases ==="
run_test "Multiple wildcards" 'echo *.*.*.c' 0
run_test "Wildcard only" 'echo *' 0
run_test "Wildcard in quotes" "echo '*'" 0
run_test "Wildcard with space" 'echo * test' 0
run_test "Question mark wildcard" 'echo ?.c' 0
run_test "Wildcard no match" 'echo *.nonexistent' 0
run_test "Wildcard at start" 'echo *test' 0
run_test "Wildcard at end" 'echo test*' 0

echo ""
echo "=== 4. Pipe Edge Cases ==="
run_test "Pipe at start" '| echo test' 1
run_test "Pipe at end" 'echo test |' 1
run_test "Multiple pipes" 'echo test | | cat' 1
run_test "Pipe with no command" 'echo |' 1
run_test "Many pipes" 'echo a | cat | cat | cat | cat | cat | cat' 0
run_test "Pipe with empty command" 'echo test |  | cat' 1

echo ""
echo "=== 5. Redirection Edge Cases ==="
run_test "Multiple input redirects" 'cat < file1 < file2' 0
run_test "Multiple output redirects" 'echo test > file1 > file2' 0
run_test "Redirect to nothing" 'echo test >' 1
run_test "Redirect from nothing" 'cat <' 1
run_test "Redirect with pipe" 'echo test > file | cat' 0
run_test "Heredoc no delimiter" 'cat <<' 1
run_test "Heredoc with quotes" "cat << 'EOF'" 0
run_test "Append redirect" 'echo test >> file' 0
run_test "Redirect to /dev/null" 'echo test > /dev/null' 0

echo ""
echo "=== 6. Logical Operators Edge Cases ==="
run_test "AND at start" '&& echo test' 1
run_test "AND at end" 'echo test &&' 1
run_test "OR at start" '|| echo test' 1
run_test "OR at end" 'echo test ||' 1
run_test "Multiple ANDs" 'true && && echo test' 1
run_test "Multiple ORs" 'false || || echo test' 1
run_test "AND and OR mix" 'true && false || echo test' 0

echo ""
echo "=== 7. Subshell Edge Cases ==="
run_test "Empty subshell" '()' 1
run_test "Unclosed subshell" '(echo test' 1
run_test "Extra close paren" 'echo test)' 1
run_test "Nested subshells" '(echo (echo test))' 0
run_test "Subshell with pipe" '(echo test) | cat' 0
run_test "Subshell with redirect" '(echo test) > file' 0

echo ""
echo "=== 8. Empty/Whitespace Cases ==="
run_test "Empty input" '' 0
run_test "Only spaces" '     ' 0
run_test "Only tabs" '\t\t\t' 0
run_test "Mixed whitespace" '  \t  \t  ' 0
run_test "Newlines only" '\n\n\n' 0

echo ""
echo "=== 9. Special Characters ==="
run_test "Semicolon" 'echo test; echo test2' 0
run_test "Backslash" 'echo \\test' 0
run_test "Tilde" 'echo ~' 0
run_test "Exclamation" 'echo !' 0
run_test "At sign" 'echo @' 0
run_test "Hash" 'echo #' 0
run_test "Percent" 'echo %' 0
run_test "Caret" 'echo ^' 0
run_test "Ampersand" 'echo &' 1

echo ""
echo "=== 10. Long Input Cases ==="
LONG_STRING=$(printf 'A%.0s' {1..1000})
run_test "Very long argument" "echo $LONG_STRING" 0
MANY_ARGS=$(printf 'arg%.0s ' {1..100})
run_test "Many arguments" "echo $MANY_ARGS" 0
run_test "Long pipe chain" 'echo test | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat' 0

echo ""
echo "=== 11. NULL and Control Characters ==="
run_test "Null byte attempt" 'echo \x00' 0
run_test "Control chars" 'echo \x01\x02\x03' 0

echo ""
echo "=== 12. Command Not Found Cases ==="
run_test "Nonexistent command" 'nonexistentcommand12345' 0
run_test "Empty command name" "''" 0

echo ""
echo "=== 13. Complex Mixed Cases ==="
run_test "Everything mixed" 'echo "test $USER" | cat > file && (echo done) || echo fail' 0
run_test "Quote and pipe" "echo 'test | fake' | cat" 0
run_test "Variable in redirect" 'echo test > $FILE' 0
run_test "Wildcard in pipe" 'echo *.c | cat' 0

echo ""
echo "========================================="
echo "RESULTS"
echo "========================================="
echo -e "PASS: ${GREEN}${PASS}${NC}"
echo -e "FAIL: ${RED}${FAIL}${NC}"
echo "TOTAL: $((PASS + FAIL))"

if [ $FAIL -eq 0 ]; then
    echo -e "\n${GREEN}All tests passed!${NC}"
    exit 0
else
    echo -e "\n${RED}Some tests failed!${NC}"
    exit 1
fi
