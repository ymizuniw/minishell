# minishell

## Basic Information

**Program name:** `minishell`  
**Turn-in files:** `Makefile`, `*.h`, `*.c`  
**Makefile rules:** `NAME`, `all`, `clean`, `fclean`, `re`  
**Authorized library:** Libft  
**Description:** Write a shell  

---

## Authorized External Functions

`readline`, `rl_clear_history`, `rl_on_new_line`, `rl_replace_line`, `rl_redisplay`, `add_history`,  
`printf`, `malloc`, `free`, `write`, `access`, `open`, `read`, `close`, `fork`,  
`wait`, `waitpid`, `wait3`, `wait4`, `signal`, `sigaction`, `sigemptyset`, `sigaddset`,  
`kill`, `exit`, `getcwd`, `chdir`, `stat`, `lstat`, `fstat`, `unlink`, `execve`,  
`dup`, `dup2`, `pipe`, `opendir`, `readdir`, `closedir`, `strerror`, `perror`,  
`isatty`, `ttyname`, `ttyslot`, `ioctl`, `getenv`, `tcsetattr`, `tcgetattr`,  
`tgetent`, `tgetflag`, `tgetnum`, `tgetstr`, `tgoto`, `tputs`

---

## Requirements

Your shell should:

- Display a prompt when waiting for a new command.  
- Have a working history.  
- Search and launch the right executable (based on the PATH variable or using a relative or an absolute path).  
- Use at most one global variable to indicate a received signal. Consider the implications: this approach ensures that your signal handler will not access your main data structures.

> ⚠️ **Caution:**  
> This global variable must only store the signal number and must not provide any additional information or access to data.  
> Therefore, using "norm" type structures in the global scope is forbidden.

- Not interpret unclosed quotes or special characters which are not required by the subject such as `\` (backslash) or `;` (semicolon).  
- Handle `'` (single quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence.  
- Handle `"` (double quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence except for `$` (dollar sign).  

---

### Redirections

Implement the following redirections:

- `<` should redirect input.  
- `>` should redirect output.  
- `<<` should be given a delimiter, then read the input until a line containing the delimiter is seen. However, it doesn’t have to update the history!  
- `>>` should redirect output in append mode.  

---

### Pipes

- Implement pipes (`|` character).  
  The output of each command in the pipeline is connected to the input of the next command via a pipe.  

---

### Environment Variables

- Handle environment variables (`$` followed by a sequence of characters) which should expand to their values.  
- Handle `$?` which should expand to the exit status of the most recently executed foreground pipeline.  

---

### Signal Handling    // SEMI_COLON,

- Handle `ctrl-C`, `ctrl-D`, and `ctrl-\` which should behave like in bash.  
- In interactive mode:  
  - `ctrl-C` displays a new prompt on a new line.  
  - `ctrl-D` exits the shell.  
  - `ctrl-\` does nothing.  

---

### Built-in Commands

Your shell must implement the following built-in commands:

- `echo` with option `-n`  
- `cd` with only a relative or absolute path  
- `pwd` with no options  
- `export` with no options  
- `unset` with no options  
- `env` with no options or arguments  
- `exit` with no options  

---

## Cautions

> ⚠️ **Memory leaks:**  
> The `readline()` function may cause memory leaks, but you are not required to fix them.  
> However, this does not mean your own code, yes the code you wrote, can have memory leaks.

> ⚠️ **Scope:**  
> You should limit yourself to the subject description. Anything that is not asked is not required.  
> If you have any doubt about a requirement, take bash as a reference.

---

## Bonus Requirements

Your program must implement:

- `&&` and `||` with parenthesis for priorities.  
- Wildcards `*` should work for the current working directory.
