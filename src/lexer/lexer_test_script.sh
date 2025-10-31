#!/bin/bash

cc  -Wall -Wextra -Werror lexer.c lexer_test.c lexer_utils/*.c ../data_management/*.c -o lexer_unit_test
./lexer_unit_test

# cc -g -Wall -Wextra -Werror lexer.c lexer_test.c lexer_utils/*.c ../data_management/*.c -o lexer_unit_test
# gdb ./lexer_unit_test

# rm -f lexer_unit_test
