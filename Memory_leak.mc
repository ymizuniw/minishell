=================================================================
==784554==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 544 byte(s) in 1 object(s) allocated from:
    #0 0x7c356a6b4887 in __interceptor_malloc ../../../../src/libsanitizer/asan/asan_malloc_linux.cpp:145
    #1 0x5de4d5402e0e in xmalloc src/data_management/xmalloc.c:19
    #2 0x5de4d540999f in generate_envp src/executer/executor_utils/search_and_exec/generate_env.c:93
    #3 0x5de4d54076b7 in search_and_exec src/executer/executor_utils/search_and_exec/search_and_exec.c:61
    #4 0x5de4d540487a in process_command src/executer/executor_utils/ast_traversal_utils/exec_command.c:63
    #5 0x5de4d5404b2b in exec_command src/executer/executor_utils/ast_traversal_utils/exec_command.c:87
    #6 0x5de4d5403e58 in ast_traversal src/executer/executor_utils/ast_traversal_utils/ast_traversal.c:42
    #7 0x5de4d54039d6 in executor src/executer/executor.c:22
    #8 0x5de4d53fe826 in exec_one_ast src/shell_loop_utils.c:79
    #9 0x5de4d53fe26a in parse_and_exec src/parse_exec.c:32
    #10 0x5de4d53fd874 in process_line src/main.c:32
    #11 0x5de4d53fdbbb in shell_loop src/main.c:87
    #12 0x5de4d53fdcee in main src/main.c:101
    #13 0x7c356a229d8f in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58

Indirect leak of 2499 byte(s) in 67 object(s) allocated from:
    #0 0x7c356a6b4887 in __interceptor_malloc ../../../../src/libsanitizer/asan/asan_malloc_linux.cpp:145
    #1 0x5de4d5402e0e in xmalloc src/data_management/xmalloc.c:19
    #2 0x5de4d5402e4a in xcalloc src/data_management/xmalloc.c:29
    #3 0x5de4d5409702 in create_env_string src/executer/executor_utils/search_and_exec/generate_env.c:46
    #4 0x5de4d5409814 in add_env_to_array src/executer/executor_utils/search_and_exec/generate_env.c:64
    #5 0x5de4d54099c4 in generate_envp src/executer/executor_utils/search_and_exec/generate_env.c:96
    #6 0x5de4d54076b7 in search_and_exec src/executer/executor_utils/search_and_exec/search_and_exec.c:61
    #7 0x5de4d540487a in process_command src/executer/executor_utils/ast_traversal_utils/exec_command.c:63
    #8 0x5de4d5404b2b in exec_command src/executer/executor_utils/ast_traversal_utils/exec_command.c:87
    #9 0x5de4d5403e58 in ast_traversal src/executer/executor_utils/ast_traversal_utils/ast_traversal.c:42
    #10 0x5de4d54039d6 in executor src/executer/executor.c:22
    #11 0x5de4d53fe826 in exec_one_ast src/shell_loop_utils.c:79
    #12 0x5de4d53fe26a in parse_and_exec src/parse_exec.c:32
    #13 0x5de4d53fd874 in process_line src/main.c:32
    #14 0x5de4d53fdbbb in shell_loop src/main.c:87
    #15 0x5de4d53fdcee in main src/main.c:101
    #16 0x7c356a229d8f in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58

おそらく、親プロセスのミニシェルのリードループが継続していて、出力バッファにプロンプトと改行が蓄積している。
これを防ぐためには、子プロセスの実行中には、シグナルを受け付けないように設定する必要がある。
