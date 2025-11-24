#!/bin/bash

# Complex expansion and quote test script
# Tests combinations of dollar expansion, quotes, and wildcards

MINISHELL="./minishell"
PASSED=0
FAILED=0

# Color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

# Test function
test_case() {
    local description="$1"
    local input="$2"
    local expected_pattern="$3"
    
    # Run command in both bash and minishell
    bash_output=$(bash -c "$input" 2>&1)
    mini_output=$(echo "$input" | $MINISHELL 2>&1 | grep -v "^minishell\$" | grep -v "^exit$")
    
    # Compare outputs
    if [[ "$bash_output" == "$mini_output" ]]; then
        echo -e "${GREEN}[PASS]${NC} $description"
        ((PASSED++))
    else
        echo -e "${RED}[FAIL]${NC} $description"
        echo "  Input:    $input"
        echo "  Expected: $bash_output"
        echo "  Got:      $mini_output"
        ((FAILED++))
    fi
}

# Setup test environment
export TEST_VAR="hello"
export USER="testuser"
export NAME="alice"

echo "==== Dollar Expansion with Quotes ===="

# Test $'...' (dollar followed by single quote)
test_case "Dollar before single quote" \
    "echo \$'a'" \
    "a"

test_case "Dollar before single quote with variable name" \
    "echo \$'USER'" \
    "USER"

# Test $"..." (dollar followed by double quote)
test_case "Dollar before double quote" \
    "echo \$\"hello\"" \
    "hello"

# Test mixed quotes with dollar
test_case "Mixed quotes: single then double" \
    "echo 'hello'\"\$USER\"" \
    "hello\$USER"

test_case "Mixed quotes: dollar variable in double quotes" \
    "echo \"Hello \$NAME\"" \
    "Hello alice"

test_case "Mixed quotes: dollar variable followed by single quote" \
    "echo \$NAME'world'" \
    "aliceworld"

# Test dollar expansion at word boundaries
test_case "Dollar at end followed by quote" \
    "echo x\$'y'" \
    "xy"

test_case "Dollar in middle with quotes" \
    "echo a\$'b'c" \
    "abc"

test_case "Multiple dollars with quotes" \
    "echo \$'a'\$'b'" \
    "ab"

# Test lone dollar
test_case "Lone dollar" \
    "echo \$" \
    ""

test_case "Lone dollar with text" \
    "echo \$text" \
    "text"

test_case "Dollar followed by space" \
    "echo \$ text" \
    " text"

echo ""
echo "==== Wildcard with Quotes and Dollar ===="

# Create test files
mkdir -p /tmp/minishell_test
cd /tmp/minishell_test
touch file1.txt file2.txt test.c

# Test wildcards (these should NOT expand in quotes)
test_case "Wildcard in single quotes" \
    "cd /tmp/minishell_test && echo '*.txt'" \
    "*.txt"

test_case "Wildcard in double quotes" \
    "cd /tmp/minishell_test && echo \"*.txt\"" \
    "*.txt"

# Test wildcard with dollar
test_case "Dollar before wildcard pattern" \
    "cd /tmp/minishell_test && echo \$'*'.txt" \
    "*.txt"

# Cleanup
cd - > /dev/null
rm -rf /tmp/minishell_test

echo ""
echo "==== Complex Combined Cases ===="

# Complex combinations
test_case "Quote, dollar, quote combination" \
    "echo 'a'\$NAME\"b\"" \
    "aaliceb"

test_case "Empty dollar expansion with quotes" \
    "echo \$NONEXISTENT'test'" \
    "test"

test_case "Multiple expansions in one word" \
    "echo \$NAME\$NAME" \
    "alicealice"

test_case "Dollar in double quotes with single quotes around" \
    "echo '\"\$NAME\"'" \
    '"\$NAME"'

# Test special characters
test_case "Dollar with special variable ?" \
    "echo \$?" \
    "0"

test_case "Dollar with special variable $ (PID)" \
    "bash -c 'echo \$\$' | wc -c" \
    "bash -c 'echo \$\$' | wc -c"

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
