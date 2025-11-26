#!/bin/bash

echo "=============test for reveiew===================="

echo "=============simple command======================"

echo "=========Abs Path=========="
/bin/ls
/bin/echo
/bin/touch in
/bin/cat in
/bin/echo "infile" >>in
/bin/cat in
/bin/rm -f in

echo "=========Rel Path=========="
ls
echo
touch in
cat in
echo "infile" >> in
cat in
rm -f in

echo "==========cd abs/rel/~==============="
cd .. && pwd cd test && pwd
tmp_path=$PWD
cd $HOME && pwd
cd tmp_path && pwd
cd ~ && pwd
cd tmp_path && pwd

echo "=============with several argments====================="
echo a a
cat a a
ls a a
touch in && echo "infile">in
touch in2 && echo "infile2">in2
cat in in2
ls src includes
rm -f in in2

echo "=============pipe====================="
echo a a | cat || echo "cat failed"
cat a a | cat || echo "cat failed"
ls a a | cat
touch in && echo "infile">in
touch in2 && echo "infile2">in2
cat in in2 | cat
rm -f in in2

echo "============Environment Variable================"
echo $USER $? $HOME
echo "$USER" '$?' "$H"OME
echo $USER$?$HOME
echo "'$USER' $HOME"
echo '"$USER" $HOME'


echo "============Unset Path==============="
INPUT="
unset $PATH
echo $PATH
touch in
/bin/touch in && echo "infile" > in
cat in || echo "cat failed"
/bin/cat in || echo "cat failed"
exit
"
cd ..
echo $INPUT | ./minishell
cd test

echo "============Set Path================="
mkdir test1 && mkdir test2
touch /test1/test1.sh && touch /test2/test1.sh
TEST1_FILE="/test1/test1.sh"
TEST2_FILE="/test2/test1.sh"
chmod +x $TEST1_FILE
chmod +x $TEST2_FILE
echo "#!/bin/bash" >> $TEST1_FILE
echo "#!/bin/bash" >> $TEST2_FILE
echo "
">> $TEST1_FILE
echo "
">> $TEST2_FILE
echo "echo test1's test">>$TEST1_FILE
echo "echo test2's test">>$TEST2_FILE
TEST1_PATH="/$PWD/$TEST1_FILE"
TEST2_PATH="/$PWD/$TEST2_FILE"
export PATH="$TEST1_PATH:$TEST2_PATH:$PATH"
./test1
export PATH="$TEST2_PATH:$TEST1_PATH:$PATH"
./test2