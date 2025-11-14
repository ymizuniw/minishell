#!/usr/bin/env bash
# Destructive Test Cases - メモリ破壊、境界値テスト

MS_BIN="./minishell"

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

run_destructive_test() {
    local name=$1
    local input=$2
    printf "%-60s " "$name"
    
    output=$(printf "%b\nexit\n" "$input" | timeout 3 $MS_BIN 2>&1)
    rc=$?
    
    if [ $rc -eq 124 ]; then
        echo -e "${RED}[TIMEOUT/HANG]${NC}"
        echo "  Input: $input" | head -c 100
        return 1
    elif [ $rc -eq 139 ]; then
        echo -e "${RED}[SEGFAULT]${NC}"
        echo "  Input: $input" | head -c 100
        return 1
    elif [ $rc -gt 128 ] && [ $rc -ne 255 ]; then
        # 255 is a valid exit code, only consider it crash if signal > 127 and not 255
        echo -e "${RED}[CRASH sig=$((rc-128))]${NC}"
        echo "  Input: $input" | head -c 100
        return 1
    else
        echo -e "${GREEN}[OK]${NC}"
        return 0
    fi
}

echo "========================================="
echo "DESTRUCTIVE TEST CASES"
echo "========================================="
echo ""

FAIL=0

echo "=== NULL/Empty Pointer Tests ==="
run_destructive_test "Empty command after pipe" "echo test | " || ((FAIL++))
run_destructive_test "Empty command before pipe" " | cat" || ((FAIL++))
run_destructive_test "Only pipe" "|" || ((FAIL++))
run_destructive_test "Only AND" "&&" || ((FAIL++))
run_destructive_test "Only OR" "||" || ((FAIL++))
run_destructive_test "Only redirect" ">" || ((FAIL++))
run_destructive_test "Only here redirect" "<" || ((FAIL++))

echo ""
echo "=== Buffer Overflow Tests ==="
# 非常に長い入力
HUGE=$(printf 'A%.0s' {1..10000})
run_destructive_test "10K character input" "echo $HUGE" || ((FAIL++))

# 非常に多くのトークン
MANY_TOKENS=$(printf 'arg%.0s ' {1..1000})
run_destructive_test "1000 tokens" "echo $MANY_TOKENS" || ((FAIL++))

# 深いネスト
DEEP_PAREN="echo test"
for i in {1..50}; do
    DEEP_PAREN="($DEEP_PAREN)"
done
run_destructive_test "50-level deep parentheses" "$DEEP_PAREN" || ((FAIL++))

echo ""
echo "=== Use-After-Free Tests ==="
run_destructive_test "Multiple expansions" 'echo $USER $USER $USER $HOME $HOME $PATH $PATH' || ((FAIL++))
run_destructive_test "Variable in variable" 'A=B; B=C; echo $A' || ((FAIL++))

echo ""
echo "=== Format String Tests ==="
run_destructive_test "Percent in echo" 'echo %s%s%s%s%s' || ((FAIL++))
run_destructive_test "Percent in variable" 'A=%s; echo $A' || ((FAIL++))

echo ""
echo "=== Integer Overflow Tests ==="
run_destructive_test "Very large exit code" 'exit 2147483647' || ((FAIL++))
run_destructive_test "Negative exit code" 'exit -1' || ((FAIL++))
run_destructive_test "Overflow exit code" 'exit 999999999999' || ((FAIL++))

echo ""
echo "=== Race Condition Tests ==="
for i in {1..20}; do
    printf "Race test %d/20\r" $i
    echo 'echo test' | $MS_BIN > /dev/null 2>&1 &
    echo 'echo test2' | $MS_BIN > /dev/null 2>&1 &
done
wait
echo -e "\n${GREEN}Race condition tests completed${NC}"

echo ""
echo "=== File System Stress ==="
run_destructive_test "Redirect to /dev/full" 'echo test > /dev/full' || ((FAIL++))
run_destructive_test "Read from /dev/random" 'cat /dev/random | head -c 10' || ((FAIL++))
run_destructive_test "Redirect to /dev/zero" 'cat /dev/zero | head -c 10 > /dev/null' || ((FAIL++))

echo ""
echo "=== Memory Allocation Failures ==="
# 巨大なheredoc
run_destructive_test "Huge heredoc" 'cat << EOF
$(printf "line\n%.0s" {1..10000})
EOF' || ((FAIL++))

# 巨大なパイプチェーン
HUGE_PIPE="echo test"
for i in {1..100}; do
    HUGE_PIPE="$HUGE_PIPE | cat"
done
run_destructive_test "100-stage pipe" "$HUGE_PIPE" || ((FAIL++))

echo ""
echo "=== Recursive/Circular Tests ==="
run_destructive_test "Self-referencing env" 'export A=$A; echo $A' || ((FAIL++))

echo ""
echo "=== Special File Descriptors ==="
run_destructive_test "Redirect stderr" 'echo test 2>&1' || ((FAIL++))
run_destructive_test "Redirect to FD 3" 'echo test 3> file' || ((FAIL++))

echo ""
echo "=== Malformed Syntax ==="
run_destructive_test "Incomplete heredoc" 'cat << EOF' || ((FAIL++))
run_destructive_test "Broken pipe chain" 'echo | | cat' || ((FAIL++))
run_destructive_test "Broken AND chain" 'true && && false' || ((FAIL++))
run_destructive_test "Broken OR chain" 'false || || true' || ((FAIL++))
run_destructive_test "Mismatched parens 1" '(echo test' || ((FAIL++))
run_destructive_test "Mismatched parens 2" 'echo test)' || ((FAIL++))
run_destructive_test "Double pipe" 'echo test || cat' || ((FAIL++))

echo ""
echo "=== Unicode and Special Chars ==="
run_destructive_test "Unicode in command" 'echo こんにちは' || ((FAIL++))
run_destructive_test "Emoji in command" 'echo 🚀🎉' || ((FAIL++))
run_destructive_test "Zero-width chars" 'echo test​test' || ((FAIL++))

echo ""
echo "=== Path Traversal ==="
run_destructive_test "Path with .." 'cat ../../../../../../etc/passwd' || ((FAIL++))
run_destructive_test "Absolute path" 'cat /etc/hosts' || ((FAIL++))

echo ""
echo "========================================="
if [ $FAIL -eq 0 ]; then
    echo -e "${GREEN}All destructive tests passed!${NC}"
else
    echo -e "${RED}$FAIL tests failed!${NC}"
fi
echo "========================================="

exit $FAIL
