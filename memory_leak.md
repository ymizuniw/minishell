
=================================================================
==841354==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 48 byte(s) in 1 object(s) allocated from:
    #0 0x73114a0b4887 in __interceptor_malloc ../../../../src/libsanitizer/asan/asan_malloc_linux.cpp:145
    #1 0x569a14a33e2e in xmalloc src/data_management/xmalloc.c:7
    #2 0x569a14a33385 in alloc_token src/data_management/alloc_t.c:16
    #3 0x569a14a3b1e2 in init_token src/lexer/lexer.c:11
    #4 0x569a14a3b320 in lexer src/lexer/lexer.c:27
    #5 0x569a14a2e788 in process_line src/main.c:10
    #6 0x569a14a2ebdb in shell_loop src/main.c:75
    #7 0x569a14a2ed0e in main src/main.c:89
    #8 0x731149c29d8f in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58

this if from tokenization process.
The token_list is free()ed? in the t_shell.
