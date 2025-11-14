Premise: still reachable memory is plenty of when cat in | cat | cat, multi-pipe.

1.handle_meta_char() generates strdup(""), then it is not free() ed. handle-eof also generates still reachable allocation.

2.parse_simple_command() generates alloc_command_node()? and it won't free()ed.

3. word_cat() generates plain_text, and the ft_realloc()ed memory is still reachable.

4.histry's element, ft_strdup()ed, but not free()ed. 

5. gen_command_node() generates data allocation, by alloc_cmd() but not free() ed.

6. getcwd() return value, is not free()ed, maybe after execve(), child process lose tracks of it.

7. update_existing_var() generates char *ptr, but not free()ed.

8.in lexer(), init_token() generates t_token by alloc_token() but not free()ed.

9. swap_with_parent() generates node by alloc_node but not free()ed.

10. 


<solution1>
for some of them, by free() ing before execve(), decrease the number of still reachable memory.
