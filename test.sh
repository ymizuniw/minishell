#!/bin/bash

ls | grep "in" || touch in && echo "infile">in;
echo "==========================multiple-pipe=========================="
cat in | cat | cat | cat | cat |cat | cat;
echo $?

echo "==========================subshell==============================="
(cat in && echo "cat in in subshell success\n") || echo "subshell failed"

echo "=========================quotation==============================="
echo "double-quotation"
echo "string string"
echo 'string string'
echo "env:USER:" "$USER"
echo "env:USER:" '$USER'
echo "nested s-quote in d-quote:" "'$USER'"
echo "nested d-quote in s-quote:" '"$USER"'
echo "complex combination1: " 'string $USER"string"'
echo "complex combination2: " "string $USER'string'"
echo "test is working?"
echo "$?"

echo "=======================wildcard==================================="
echo "wildcard mono:" *
echo "wildcard s-quoted:" '*'
echo "wildcard d-quoted:" "*"
ls | grep "pattern0001" || touch pattern0001 && echo "pattern0001 content.">pattern0001;
echo "wildcard pattern-matching:" pa*1
echo "wildcard s-quoted:" 'pa*1'
echo "wildcard d-quoted:" "pa*1"
echo "wildcard pattern-matching:" *a*01
echo "wildcard pattern-matching:" ***01
echo "wildcard pattern-matching:" ****1
echo "wildcard pattern-matching:" *****
echo "$?"

echo "========================subshell=================================="
echo "process AND_IF:"
(cat in && rm -f in) && echo "cat success"
echo "process OR_IF:"
(cat non && echo "cat success. test is wrong.") || echo "cat failed"
