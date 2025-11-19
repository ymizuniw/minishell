#!/bin/bash

cd ..
make fclean -j$(nproc) > test/make.log
make -j$(nproc) > test/make.log
V_FLAG="valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes --track-fds=yes -q --log-file=test/valg.log"
$V_FLAG ./minishell
cd test
./test_integration.sh