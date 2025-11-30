#!/bin/bash

echo "============= test for review ===================="

echo "============= simple command ====================="

echo "========= Abs Path =========="
/bin/ls
/bin/echo
/bin/touch in
/bin/cat in
/bin/echo "infile" >> in
/bin/cat in
/bin/rm -f in

echo "========= Rel Path =========="
ls
echo
touch in
echo "infile" > in
cat in
rm -f in

echo "========== cd abs / rel / ~ ==============="
cd .. && pwd
cd test && pwd
cd "$HOME" && pwd
cd ~ && pwd

echo "============= several arguments ====================="
echo a a
cat a a || echo "cat failed"
ls a a || echo "ls failed"
touch in && echo "infile" > in
touch in2 && echo "infile2" > in2
cat in in2
ls src includes || echo "ls failed"
rm -f in in2

echo "============= pipe ====================="
echo a a | cat
cat a a | cat || echo "cat failed"
ls a a | cat
touch in && echo "infile" > in
touch in2 && echo "infile2" > in2
cat in in2 | cat
rm -f in in2

echo "============ Environment Variable ================"
echo $USER $? $HOME
echo "$USER" '$?' "$HOME"
echo "'$USER' $HOME"

echo "============ Unset Path ==============="
INPUT=$(cat << 'EOF'
unset PATH
echo $PATH
/bin/touch in && echo "infile" > in
/bin/cat in
exit
EOF
)

cd ..
echo "$INPUT" | ./minishell
cd test

echo "============ Set Path ==============="

mkdir -p test1
mkdir -p test2

echo "#!/bin/bash" > test1/test1.sh
echo "echo test1's test" >> test1/test1.sh
chmod +x test1/test1.sh

echo "#!/bin/bash" > test2/test2.sh
echo "echo test2's test" >> test2/test2.sh
chmod +x test2/test2.sh

TEST1_PATH="$PWD/test1"
TEST2_PATH="$PWD/test2"

export PATH="$TEST1_PATH:$PATH"
test1.sh

export PATH="$TEST2_PATH:$PATH"
test2.sh
