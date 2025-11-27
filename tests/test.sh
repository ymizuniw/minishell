#!/bin/bash

echo "==========================file setup=========================="
ls | grep "in" || (touch in && echo "infile" > in)
echo $?

echo "==========================multiple-pipe=========================="
cat in | cat | cat | cat | cat | cat | cat
echo $?

echo "==========================subshell==============================="
( cat in && echo "cat in in subshell success" ) || echo "subshell failed"
echo $?

echo "=========================quotation==============================="
echo "double-quotation"
echo $?
echo "string string"
echo $?
echo 'string string'
echo $?
echo "env:USER:" "$USER"
echo $?
echo "env:USER:" '$USER'
echo $?
echo "nested s-quote in d-quote:" "'$USER'"
echo $?
echo "nested d-quote in s-quote:" '"$USER"'
echo $?
echo "complex combination1:" 'string $USER"string"'
echo $?
echo "complex combination2:" "string $USER'string'"
echo $?
echo "test is working?"
echo "$?"

echo "=======================wildcard==================================="
echo "wildcard mono:" *
echo $?
echo "wildcard s-quoted:" '*'
echo $?
echo "wildcard d-quoted:" "*"
echo $?

ls | grep "pattern0001" || (touch pattern0001 && echo "pattern0001 content." > pattern0001)
echo $?

echo "wildcard pattern-matching:" pa*1
echo $?
echo "wildcard s-quoted:" 'pa*1'
echo $?
echo "wildcard d-quoted:" "pa*1"
echo $?
echo "wildcard pattern-matching:" *a*01
echo $?
echo "wildcard pattern-matching:" ***01   # "*" "*" "*01"
echo $?
echo "wildcard pattern-matching:" ****1    # 4つの "*" + "1"
echo $?
echo "wildcard pattern-matching:" *****    # "*" 5個展開
echo "$?"

echo "========================subshell (logical ops)======================"
echo "process AND_IF:"
( cat in && rm -f in ) && echo "cat success"
echo $?

echo "process OR_IF:"
( cat non && echo "cat success, this should be wrong" ) || echo "cat failed"
echo $?

echo "========================heredoc==================================="
echo "multiple heredoc:"
TEST=$(cat <<EOF && cat <<EOF && cat <<EOF | sort -u
abc
geh
def
abc
EOF
1
2
3
EOF
p
q
r
EOF
)
echo "$TEST"
echo $?

echo "quoted heredoc:"
TEST=$(cat <<EOF && cat <<E"O"F && cat <<"EOF" && cat <<'EOF' && cat <<'EOF'
$USER
EOF
$USER
EOF
$USER
EOF
$USER
EOF
$USER
EOF
)
echo "$TEST"
echo $?

echo "=========================syntax error tests============================="

echo "quotation not closed."
echo $?
echo 'aaa
echo $?
echo \"aaa
echo $?
echo aaa\'
echo $?
echo aaa\"
echo $?

echo "parenthesis not closed, empty."
echo $?
( aaa
echo $?
aaa )
echo $?
()
echo $?
(())
echo $?
('('))
echo $?

echo "redirection not proper assignment."
echo << EOF && echo "redirection AND_IF"
EOF
echo $?

echo > notafile || echo "redirection OR_IF"
echo $?

( echo << EOF ; echo "subshell redirection"; ) 2>/dev/null
EOF

echo << EOF * && echo "redirection wildcard"
EOF
