#!/bin/bash

ls | grep "in" || touch in && echo "infile">in;
echo $?
echo "==========================multiple-pipe=========================="
cat in | cat | cat | cat | cat |cat | cat;
echo $?

echo "==========================subshell==============================="
(cat in && echo "cat in in subshell success\n") || echo "subshell failed"
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
echo "complex combination1: " 'string $USER"string"'
echo $?
echo "complex combination2: " "string $USER'string'"
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
ls | grep "pattern0001" || touch pattern0001 && echo "pattern0001 content.">pattern0001;
echo $?
echo "wildcard pattern-matching:" pa*1
echo $?
echo "wildcard s-quoted:" 'pa*1'
echo $?
echo "wildcard d-quoted:" "pa*1"
echo $?
echo "wildcard pattern-matching:" *a*01
echo $?
echo "wildcard pattern-matching:" ***01
echo $?
echo "wildcard pattern-matching:" ****1
echo $?
echo "wildcard pattern-matching:" *****
echo "$?"

echo "========================subshell=================================="
echo "process AND_IF:"
(cat in && rm -f in) && echo "cat success"
echo $?
echo "process OR_IF:"
(cat non && echo "cat success. test is wrong.") || echo "cat failed"
echo $?

echo "========================heredoc==================================="
echo "multiple heredoc:"
TEST=$(cat <<EOF && cat <<EOF && cat <<EOF | cat
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
echo $TEST
echo $?

echo "quoted heredoc:"
TEST=$(cat <<EOF && cat <<E"O"F && cat <<"EOF" && cat <<'E'OF && cat <<'EOF'
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
echo $TEST
echo $?

# echo "=========================syntax error============================="
# echo "quotation not closed."

# echo 'aaa
# echo $?
# \"aaa
# echo $?
# aaa\'
# echo $?
# aaa\"
# echo $?
# # "aaa"'
# # echo $?
# # 'aaa'"
# echo $?
# "'aaa'""
# echo $?
# '"aaa"''
# echo $?

# echo "parenthesis not closed, empty."
# echo $?
# (aaa
# echo $?
# aaa)
# echo $?
# ()
# echo $?
# (())
# echo $?
# ('(')')'
# echo $?

# echo "redirection not proper assignment."
# echo << && echo "redirection AND_IF"
# echo $?
# echo > || echo "redirection OR_IF"
# echo $?
# echo << (echo "subshell") && echo redirection SUBSHELL

# echo << * && echo "redirection wildcard"

echo "========================redirection============================"
touch input_file && echo "input_file_line">input_file
cat <input_file >output_file && cat output_file
cat output_file >outfile2 >outfile3 && cat outfile3
rm -f input_file output_file outfile2 outfile3


echo "========================absolute_path_command=================="
/bin/ls
/bin/echo "absolute!"
/bin/ls | /bin/grep mini

echo "========================relative_path_command==================="
mkdir test1 test2
cd test1 && pwd && cd .. && pwd
cd test2 && pwd && cd .. && pwd
cd test1
touch test1.sh && chmod +x test1.sh
echo "#!bin/bash" > test1.sh
echo "echo test1" >>test1.sh
cd ..
cd test2
touch test2.sh && chmod +x test2.sh
echo "#!bin/bash">test2.sh
echo "echo test2" >>test2.sh
cd ..

