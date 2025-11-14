#!/bin/bash

# Comprehensive test suite for minishell
# Tests edge cases, error handling, and complex combinations

MINISHELL="./minishell"
PASSED=0
FAILED=0

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m'

test_case() {
    local description="$1"
    local input="$2"
    local expected="$3"
    
    result=$(echo "$input" | $MINISHELL 2>&1 | grep -v "^minishell\$" | grep -v "^exit$")
    
    if [[ "$result" == "$expected" ]]; then
        echo -e "${GREEN}[PASS]${NC} $description"
        ((PASSED++))
    else
        echo -e "${RED}[FAIL]${NC} $description"
        echo "  Input:    $input"
        echo "  Expected: $expected"
        echo "  Got:      $result"
        ((FAILED++))
    fi
}

export TEST_VAR="hello"
export NAME="alice"
export EMPTY=""

echo "==== Edge Cases: Empty and Whitespace ===="
test_case "Empty command" "" ""
test_case "Only spaces" "   " ""
test_case "Only tabs" "		" ""
test_case "Mixed whitespace" "  	  " ""

echo ""
echo "==== Edge Cases: Quotes ===="
test_case "Empty single quotes" "echo ''" ""
test_case "Empty double quotes" "echo \"\"" ""
test_case "Multiple empty quotes" "echo '' \"\" ''" "  "
test_case "Nested quotes - single in double" "echo \"'hello'\"" "'hello'"
test_case "Nested quotes - double in single" "echo '\"hello\"'" '"hello"'
test_case "Unclosed single quote treated as literal" "echo 'hello" "'hello"
test_case "Unclosed double quote treated as literal" "echo \"hello" '"hello'

echo ""
echo "==== Edge Cases: Dollar Expansion ===="
test_case "Empty variable expansion" "echo \$EMPTY" ""
test_case "Nonexistent variable" "echo \$NONEXISTENT" ""
test_case "Dollar at end of line" "echo test\$" "test\$"
test_case "Dollar with space" "echo \$ hello" "$ hello"
test_case "Double dollar" "echo \$\$" "\$\$"
test_case "Dollar with special chars" "echo \$@" ""
test_case "Dollar with digit" "echo \$0" ""
test_case "Dollar with question mark" "echo \$?" "0"
test_case "Variable in middle" "echo pre\${NAME}post" "prealicepost"
test_case "Multiple variables" "echo \$NAME \$TEST_VAR" "alice hello"

echo ""
echo "==== Complex Quote + Dollar Combinations ===="
test_case "Variable in double quotes" "echo \"\$NAME\"" "alice"
test_case "Variable in single quotes (no expand)" "echo '\$NAME'" "\$NAME"
test_case "Mixed: var + single quote" "echo \$NAME'test'" "alicetest"
test_case "Mixed: var + double quote" "echo \$NAME\"test\"" "alicetest"
test_case "Mixed: single + var + double" "echo 'a'\$NAME\"b\"" "aaliceb"
test_case "Empty var with quotes" "echo \$EMPTY'test'" "test"
test_case "Dollar in double with text" "echo \"Hello \$NAME!\"" "Hello alice!"
test_case "Multiple vars in double quotes" "echo \"\$NAME and \$TEST_VAR\"" "alice and hello"

echo ""
echo "==== Wildcard Tests ===="
# Create test files
mkdir -p /tmp/minishell_wc_test
cd /tmp/minishell_wc_test
touch file1.txt file2.txt test.c readme.md

test_case "Simple wildcard" "cd /tmp/minishell_wc_test && echo *.txt" "file1.txt file2.txt"
test_case "Wildcard with prefix" "cd /tmp/minishell_wc_test && echo file*.txt" "file1.txt file2.txt"
test_case "Wildcard with suffix" "cd /tmp/minishell_wc_test && echo *.md" "readme.md"
test_case "Multiple wildcards" "cd /tmp/minishell_wc_test && echo *.txt *.c" "file1.txt file2.txt test.c"
test_case "No match wildcard" "cd /tmp/minishell_wc_test && echo *.xyz" "*.xyz"
test_case "Wildcard in single quotes" "cd /tmp/minishell_wc_test && echo '*.txt'" "*.txt"
test_case "Wildcard in double quotes" "cd /tmp/minishell_wc_test && echo \"*.txt\"" "*.txt"
test_case "Question mark wildcard" "cd /tmp/minishell_wc_test && echo file?.txt" "file1.txt file2.txt"

cd - > /dev/null
rm -rf /tmp/minishell_wc_test

echo ""
echo "==== Complex Mixed Cases ===="
test_case "Quote + wildcard + var" "echo 'test' \$NAME *.nonexist" "test alice *.nonexist"
test_case "Escaped dollar with quote" "echo \\\$'NAME'" "\$NAME"
test_case "Multiple quote types mixed" "echo 'a'\"b\"'c'" "abc"
test_case "Empty between quotes" "echo 'a''b'" "ab"
test_case "Space in quotes preserved" "echo 'a b c'" "a b c"
test_case "Tab in quotes preserved" "echo 'a	b'" "a	b"
test_case "Newline handling" "echo 'test'" "test"

echo ""
echo "==== Special Characters ===="
test_case "Exclamation mark" "echo '!'" "!"
test_case "Hash symbol" "echo '#comment'" "#comment"
test_case "Percent sign" "echo '%'" "%"
test_case "Ampersand in quotes" "echo '&'" "&"
test_case "Asterisk in quotes" "echo '*'" "*"
test_case "Plus sign" "echo '+'" "+"
test_case "Equals sign" "echo '='" "="
test_case "Brackets" "echo '[]'" "[]"
test_case "Braces" "echo '{}'" "{}"
test_case "Parentheses" "echo '()'" "()"

echo ""
echo "==== Long Inputs ===="
long_str=$(printf 'a%.0s' {1..100})
test_case "100 characters" "echo $long_str" "$long_str"
test_case "Long variable name" "echo \$NAME\$NAME\$NAME\$NAME\$NAME" "alicealicealicealicealice"
test_case "Many quotes" "echo 'a''b''c''d''e'" "abcde"

echo ""
echo "==== Multiple Arguments ===="
test_case "10 args" "echo 1 2 3 4 5 6 7 8 9 10" "1 2 3 4 5 6 7 8 9 10"
test_case "Args with quotes" "echo 'a' 'b' 'c'" "a b c"
test_case "Mixed arg types" "echo \$NAME 'literal' \"double\" text" "alice literal double text"

echo ""
echo "==== Edge Cases: Pathological Inputs ===="
test_case "Only dollar signs" "echo \$ \$ \$" "$ $ $"
test_case "Only quotes" "echo '' '' ''" "  "
test_case "Alternating quotes" "echo '\"'\"'\"'" '"""'
test_case "Many dollars" "echo \$\$\$\$" "\$\$\$\$"

echo ""
echo "==== Variable Edge Cases ===="
test_case "Variable with underscore" "export _VAR=test && echo \$_VAR" "test"
test_case "Variable with number" "export VAR2=test && echo \$VAR2" "test"
test_case "Variable all caps" "export ALLCAPS=test && echo \$ALLCAPS" "test"
test_case "Variable lowercase" "export lowercase=test && echo \$lowercase" "test"

echo ""
echo "==== Combination Stress Tests ===="
test_case "Everything mixed 1" "echo \$NAME'test'\"\$TEST_VAR\" *.xyz" "alicetesthello *.xyz"
test_case "Everything mixed 2" "echo 'a' \$EMPTY \"b\" \$NAME 'c'" "a b alice c"
test_case "Nested expansion attempt" "echo \"\$NAME\$TEST_VAR\"" "alicehello"
test_case "Quote after dollar" "echo \$'NAME'" "NAME"
test_case "Double quote after dollar" "echo \$\"NAME\"" "NAME"

echo ""
echo "==== Summary ===="
echo "Total: $((PASSED + FAILED))"
echo -e "${GREEN}Passed: $PASSED${NC}"
echo -e "${RED}Failed: $FAILED${NC}"

if [ $FAILED -eq 0 ]; then
    exit 0
else
    exit 1
fi
