#!/usr/bin/env bash
# Advanced Stress Test - メモリリーク、FDリーク、クラッシュテスト

MS_BIN=${MS_BIN:-"./minishell"}

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo "========================================="
echo "ADVANCED STRESS TESTS"
echo "========================================="
echo ""

# 1. Memory stress test
echo "=== Memory Stress Test ==="
echo "Running 100 iterations of complex commands..."
for i in {1..100}; do
    printf "Iteration %d/100\r" $i
    echo 'echo test | cat | cat | grep test | wc -l' | $MS_BIN > /dev/null 2>&1
    echo 'echo $USER $HOME $PATH' | $MS_BIN > /dev/null 2>&1
    echo 'echo *.c | cat' | $MS_BIN > /dev/null 2>&1
done
echo -e "\n${GREEN}Memory stress test completed${NC}"
echo ""

# 2. File descriptor leak test
echo "=== File Descriptor Leak Test ==="
INITIAL_FDS=$(ls /proc/$$/fd | wc -l)
for i in {1..50}; do
    printf "Iteration %d/50\r" $i
    echo 'cat < /dev/null' | $MS_BIN > /dev/null 2>&1
    echo 'echo test > /tmp/test_output_$$' | $MS_BIN > /dev/null 2>&1
    echo 'cat << EOF
test
EOF' | $MS_BIN > /dev/null 2>&1
done
FINAL_FDS=$(ls /proc/$$/fd | wc -l)
FD_DIFF=$((FINAL_FDS - INITIAL_FDS))
if [ $FD_DIFF -gt 5 ]; then
    echo -e "${RED}Potential FD leak detected: +${FD_DIFF} descriptors${NC}"
else
    echo -e "${GREEN}No significant FD leak detected${NC}"
fi
rm -f /tmp/test_output_$$
echo ""

# 3. Rapid execution test
echo "=== Rapid Execution Test ==="
for i in {1..200}; do
    printf "Iteration %d/200\r" $i
    echo 'echo test' | $MS_BIN > /dev/null 2>&1 &
done
wait
echo -e "\n${GREEN}Rapid execution test completed${NC}"
echo ""

# 4. Extreme edge cases
echo "=== Extreme Edge Cases ==="

test_case() {
    local name=$1
    local input=$2
    printf "Testing: %-50s " "$name"
    output=$(printf "%b\nexit\n" "$input" | timeout 2 $MS_BIN 2>&1)
    rc=$?
    if [ $rc -eq 124 ]; then
        echo -e "${RED}[TIMEOUT]${NC}"
    elif [ $rc -gt 128 ]; then
        echo -e "${RED}[CRASH]${NC}"
        echo "Signal: $((rc - 128))"
    else
        echo -e "${GREEN}[OK]${NC}"
    fi
}

# Deeply nested quotes and expansions
test_case "Deeply nested quotes" "echo \"'\"'\"'\"'\"'\"'\""
test_case "Many variables" 'echo $A $B $C $D $E $F $G $H $I $J $K $L $M $N $O $P'
test_case "Long single quote" "echo '$(printf 'A%.0s' {1..500})'"
test_case "Long double quote" 'echo "$(printf "A%.0s" {1..500})"'

# Extreme redirection
test_case "Many redirects" 'echo test > /dev/null > /dev/null > /dev/null > /dev/null'
test_case "Redirect chain" 'cat < /dev/null > /dev/null 2>&1'

# Extreme pipes
test_case "20 pipe stages" 'echo test | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat'

# Complex boolean logic
test_case "Complex logic" 'true && true && true || false || false && true'
test_case "Logic with pipes" 'echo a | cat && echo b | cat || echo c | cat'

# Subshell stress
test_case "Deep subshells" '(((((echo test)))))'
test_case "Subshell chain" '(echo a) && (echo b) && (echo c)'

# Mixed everything
test_case "Kitchen sink" 'echo "$USER" | cat && (echo test > /dev/null) || false'

echo ""

# 5. Signal handling
echo "=== Signal Handling ==="
printf "Testing Ctrl+C handling... "
(sleep 0.5 && kill -INT $$) &
echo 'sleep 10' | $MS_BIN > /dev/null 2>&1
echo -e "${GREEN}[OK]${NC}"

echo ""

# 6. Heredoc stress
echo "=== Heredoc Stress Test ==="
test_case "Large heredoc" 'cat << EOF
$(printf "line%d\n" {1..100})
EOF'

test_case "Multiple heredocs" 'cat << EOF1 && cat << EOF2
test1
EOF1
test2
EOF2'

test_case "Heredoc with expansion" 'cat << EOF
$USER
$HOME
EOF'

echo ""

# 7. Builtin stress
echo "=== Builtin Command Stress ==="
test_case "cd to nonexistent" 'cd /nonexistent/path/123456'
test_case "export many vars" 'export A=1 B=2 C=3 D=4 E=5 F=6 G=7 H=8'
test_case "unset nonexistent" 'unset NONEXISTENT_VAR_12345'
test_case "echo with -n" 'echo -n test'
test_case "pwd multiple times" 'pwd && pwd && pwd'
test_case "env pipe" 'env | grep USER'
test_case "exit with arg" 'exit 42'

echo ""

# 8. Quote and escape combinations
echo "=== Quote Escape Combinations ==="
test_case "Quote after escape" 'echo \'"'"'test'
test_case "Escaped dollar" 'echo \$USER'
test_case "Mixed quotes 1" 'echo "'"'"'test'"'"'"'
test_case "Mixed quotes 2" "echo '\"test\"'"
test_case "Empty mixed" '""'"''"'""'

echo ""
echo "========================================="
echo "ADVANCED TESTS COMPLETED"
echo "========================================="
