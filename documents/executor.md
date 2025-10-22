3.7 Executing Commands
Simple Command Expansion
Command Search and Execution
Command Execution Environment
Environment
Exit Status
Signals
Next: Command Search and Execution, Up: Executing Commands   [Contents][Index]

3.7.1 Simple Command Expansion
When the shell executes a simple command, it performs the following expansions, assignments, and redirections, from left to right, in the following order.

redirections are saved for later processing.
The words that are not redirections are expanded (see Shell Expansions). If any words remain after expansion, the first word is taken to be the name of the command and the remaining words are the arguments.

Redirections are performed as described above (see Redirections).

If no command name results, redirections are performed, but do not affect the current shell environment. A redirection error causes the command to exit with a non-zero status.

If there is a command name left after expansion, execution proceeds as described below. Otherwise, the command exits. 

Next: Command Execution Environment, Previous: Simple Command Expansion, Up: Executing Commands   [Contents][Index]



3.7.2 Command Search and Execution
After a command has been split into words, if it results in a simple command and an optional list of arguments, the shell performs the following actions.

If the command name contains no slashes, the shell attempts to locate it. If there exists a shell function by that name, that function is invoked as described in Shell Functions.

If the name does not match a function, the shell searches for it in the list of shell builtins. If a match is found, that builtin is invoked.

If the name is neither a shell function nor a builtin, and contains no slashes, Bash searches each element of $PATH for a directory containing an executable file by that name. 

Bash uses a hash table to remember the full pathnames of executable files to avoid multiple PATH searches (see the description of hash in Bourne Shell Builtins). 

Bash performs a full search of the directories in $PATH only if the command is not found in the hash table.

 If the search is unsuccessful, the shell searches for a defined shell function named command_not_found_handle. If that function exists, it is invoked in a separate execution environment with the original command and the original command’s arguments as its arguments, and the function’s exit status becomes the exit status of that subshell. 
 If that function is not defined, the shell prints an error message and returns an exit status of 127.

If the search is successful, or if the command name contains one or more slashes, the shell executes the named program in a separate execution environment. 
Argument 0 is set to the name given, and the remaining arguments to the command are set to the arguments supplied, if any.
If this execution fails because the file is not in executable format, and the file is not a directory, it is assumed to be a shell script, a file containing shell commands, and the shell executes it as described in Shell Scripts.

If the command was not begun asynchronously, the shell waits for the command to complete and collects its exit status.
Next: Environment, Previous: Command Search and Execution, Up: Executing Commands   [Contents][Index]



3.7.3 Command Execution Environment
The shell has an execution environment, which consists of the following:

Open files inherited by the shell at invocation, as modified by redirections supplied to the exec builtin.

The current working directory as set by cd, pushd, or popd, or inherited by the shell at invocation.
The file creation mode mask as set by umask or inherited from the shell’s parent.
Current traps set by trap.
Shell parameters that are inherited from the shell’s parent in the environment.
Shell functions defined during execution or inherited from the shell’s parent in the environment.
Options enabled at invocation (either by default or with command-line arguments) or by set.
Options enabled by shopt (see The Shopt Builtin).
Shell aliases defined with alias (see Aliases).
Various process IDs, including those of background jobs (see Lists of Commands), the value of $$, and the value of $PPID.
When a simple command other than a builtin or shell function is to be executed, it is invoked in a separate execution environment that consists of the following. Unless otherwise noted, the values are inherited from the shell.

The shell’s open files, plus any modifications and additions specified by redirections to the command.

The current working directory.
The file creation mode mask.
Shell variables and functions marked for export, along with variables exported for the command, passed in the environment (see Environment).

A command invoked in this separate environment cannot affect the shell’s execution environment.

A subshell is a copy of the shell process.

Command substitution, commands grouped with parentheses, and asynchronous commands are invoked in a subshell environment that is a duplicate of the shell environment, except that traps caught by the shell are reset to the values that the shell inherited from its parent at invocation. 

Builtin commands that are invoked as part of a pipeline, except possibly in the last element depending on the value of the lastpipe shell option (see The Shopt Builtin), are also executed in a subshell environment. Changes made to the subshell environment cannot affect the shell’s execution environment.

When the shell is in POSIX mode, subshells spawned to execute command substitutions inherit the value of the -e option from the parent shell. When not in POSIX mode, Bash clears the -e option in such subshells See the description of the inherit_errexit shell option (see Bash Builtin Commands) for how to control this behavior when not in POSIX mode.

If a command is followed by a ‘&’ and job control is not active, the default standard input for the command is the empty file /dev/null. Otherwise, the invoked command inherits the file descriptors of the calling shell as modified by redirections.

Next: Exit Status, Previous: Command Execution Environment, Up: Executing Commands   [Contents][Index]

3.7.4 Environment
When a program is invoked it is given an array of strings called the environment. This is a list of name-value pairs, of the form name=value.

Bash provides several ways to manipulate the environment. On invocation, the shell scans its own environment and creates a parameter for each name found, automatically marking it for export to child processes. Executed commands inherit the environment. The export, ‘declare -x’, and unset commands modify the environment by adding and deleting parameters and functions. If the value of a parameter in the environment is modified, the new value automatically becomes part of the environment, replacing the old. The environment inherited by any executed command consists of the shell’s initial environment, whose values may be modified in the shell, less any pairs removed by the unset and ‘export -n’ commands, plus any additions via the export and ‘declare -x’ commands.

If any parameter assignment statements, as described in Shell Parameters, appear before a simple command, the variable assignments are part of that command’s environment for as long as it executes. These assignment statements affect only the environment seen by that command. If these assignments precede a call to a shell function, the variables are local to the function and exported to that function’s children.

If the -k option is set (see The Set Builtin), then all parameter assignments are placed in the environment for a command, not just those that precede the command name.

When Bash invokes an external command, the variable ‘$_’ is set to the full pathname of the command and passed to that command in its environment.

Next: Signals, Previous: Environment, Up: Executing Commands   [Contents][Index]

3.7.5 Exit Status
The exit status of an executed command is the value returned by the waitpid system call or equivalent function. Exit statuses fall between 0 and 255, though, as explained below, the shell may use values above 125 specially. Exit statuses from shell builtins and compound commands are also limited to this range. Under certain circumstances, the shell will use special values to indicate specific failure modes.

For the shell’s purposes, a command which exits with a zero exit status has succeeded. So while an exit status of zero indicates success, a non-zero exit status indicates failure. This seemingly counter-intuitive scheme is used so there is one well-defined way to indicate success and a variety of ways to indicate various failure modes.

When a command terminates on a fatal signal whose number is N, Bash uses the value 128+N as the exit status.

If a command is not found, the child process created to execute it returns a status of 127. If a command is found but is not executable, the return status is 126.

If a command fails because of an error during expansion or redirection, the exit status is greater than zero.

The exit status is used by the Bash conditional commands (see Conditional Constructs) and some of the list constructs (see Lists of Commands).

All of the Bash builtins return an exit status of zero if they succeed and a non-zero status on failure, so they may be used by the conditional and list constructs. All builtins return an exit status of 2 to indicate incorrect usage, generally invalid options or missing arguments.

The exit status of the last command is available in the special parameter $? (see Special Parameters).

Bash itself returns the exit status of the last command executed, unless a syntax error occurs, in which case it exits with a non-zero value. See also the exit builtin command (see Bourne Shell Builtins.

