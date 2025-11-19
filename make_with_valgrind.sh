#!/bin/bash

make fclean -j$(nproc) > make.log
make -j$(nproc) > make.log
V_FLAG="valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes --track-fds=yes -q --log-file=valg.log"
$V_FLAG ./minishell
./test_integration.sh
