3.1.1 Shell Operation
The following is a brief description of the shell’s operation when it reads and executes a command. Basically, the shell does the following:

Reads its input from a file (see Shell Scripts), from a string supplied as an argument to the -c invocation option (see Invoking Bash), or from the user’s terminal.

Breaks the input into words and operators, obeying the quoting rules described in Quoting. These tokens are separated by metacharacters. This step performs alias expansion (see Aliases).


Parses the tokens into simple and compound commands (see Shell Commands).
Performs the various shell expansions (see Shell Expansions), breaking the expanded tokens into lists of filenames (see Filename Expansion) and commands and arguments.
Performs any necessary redirections (see Redirections) and removes the redirection operators and their operands from the argument list.
Executes the command (see Executing Commands).
Optionally waits for the command to complete and collects its exit status (see Exit Status).


3.1.2.2 Single Quotes
Enclosing characters in single quotes (‘'’) preserves the literal value of each character within the quotes. A single quote may not occur between single quotes, even when preceded by a backslash.

Next: ANSI-C Quoting, Previous: Single Quotes, Up: Quoting   [Contents][Index]

3.1.2.3 Double Quotes
Enclosing characters in double quotes (‘"’) preserves the literal value of all characters within the quotes, with the exception of ‘$’, ‘`’, ‘\’, and, when history expansion is enabled, ‘!’. When the shell is in POSIX mode (see Bash and POSIX), the ‘!’ has no special meaning within double quotes, even when history expansion is enabled. The characters ‘$’ and ‘`’ retain their special meaning within double quotes (see Shell Expansions). The backslash retains its special meaning only when followed by one of the following characters: ‘$’, ‘`’, ‘"’, ‘\’, or newline. Within double quotes, backslashes that are followed by one of these characters are removed. Backslashes preceding characters without a special meaning are left unmodified.

A double quote may be quoted within double quotes by preceding it with a backslash. If enabled, history expansion will be performed unless an ‘!’ appearing in double quotes is escaped using a backslash. The backslash preceding the ‘!’ is not removed.

The special parameters ‘*’ and ‘@’ have special meaning when in double quotes (see Shell Parameter Expansion).

3.2 Shell Commands
A simple shell command such as echo a b c consists of the command itself followed by arguments, separated by spaces.

More complex shell commands are composed of simple commands arranged together in a variety of ways: in a pipeline in which the output of one command becomes the input of a second, in a loop or conditional construct, or in some other grouping.

Reserved Words
Simple Commands
Pipelines
Lists of Commands
Compound Commands
Coprocesses
GNU Parallel

3.2.2 Simple Commands
A simple command is the kind of command that’s executed most often. It’s just a sequence of words separated by blanks, terminated by one of the shell’s control operators (see Definitions). The first word generally specifies a command to be executed, with the rest of the words being that command’s arguments.

The return status (see Exit Status) of a simple command is its exit status as provided by the POSIX 1003.1 waitpid function, or 128+n if the command was terminated by signal n.


3.2.3 Pipelines
A pipeline is a sequence of one or more commands separated by one of the control operators ‘|’ or ‘|&’.

The format for a pipeline is

[time [-p]] [!] command1 [ | or |& command2 ] ...
The output of each command in the pipeline is connected via a pipe to the input of the next command. That is, each command reads the previous command’s output. This connection is performed before any redirections specified by command1.

If ‘|&’ is the pipeline operator, command1’s standard error, in addition to its standard output, is connected to command2’s standard input through the pipe; it is shorthand for 2>&1 |. This implicit redirection of the standard error to the standard output is performed after any redirections specified by command1, consistent with that shorthand.

If the reserved word time precedes the pipeline, Bash prints timing statistics for the pipeline once it finishes. The statistics currently consist of elapsed (wall-clock) time and user and system time consumed by the command’s execution. The -p option changes the output format to that specified by POSIX. When the shell is in POSIX mode (see Bash and POSIX), it does not recognize time as a reserved word if the next token begins with a ‘-’. The value of the TIMEFORMAT variable is a format string that specifies how the timing information should be displayed. See Bash Variables, for a description of the available formats. Providing time as a reserved word permits the timing of shell builtins, shell functions, and pipelines. An external time command cannot time these easily.

When the shell is in POSIX mode (see Bash and POSIX), you can use time by itself as a simple command. In this case, the shell displays the total user and system time consumed by the shell and its children. The TIMEFORMAT variable specifies the format of the time information.

If a pipeline is not executed asynchronously (see Lists of Commands), the shell waits for all commands in the pipeline to complete.

Each command in a multi-command pipeline, where pipes are created, is executed in its own subshell, which is a separate process (see Command Execution Environment). If the lastpipe option is enabled using the shopt builtin (see The Shopt Builtin), and job control is not active, the last element of a pipeline may be run by the shell process.

The exit status of a pipeline is the exit status of the last command in the pipeline, unless the pipefail option is enabled (see The Set Builtin). If pipefail is enabled, the pipeline’s return status is the value of the last (rightmost) command to exit with a non-zero status, or zero if all commands exit successfully. If the reserved word ‘!’ precedes the pipeline, the exit status is the logical negation of the exit status as described above. If a pipeline is not executed asynchronously (see Lists of Commands), the shell waits for all commands in the pipeline to terminate before returning a value. The return status of an asynchronous pipeline is 0.

AND and OR lists are sequences of one or more pipelines separated by the control operators ‘&&’ and ‘||’, respectively. AND and OR lists are executed with left associativity.

An AND list has the form

command1 && command2
command2 is executed if, and only if, command1 returns an exit status of zero (success).

An OR list has the form

command1 || command2
command2 is executed if, and only if, command1 returns a non-zero exit status.

The return status of AND and OR lists is the exit status of the last command executed in the list.


expression1 && expression2
True if both expression1 and expression2 are true.

expression1 || expression2
True if either expression1 or expression2 is true.



/*
below will be used?
*/
3.2.5.3 Grouping Commands
Bash provides two ways to group a list of commands to be executed as a unit. When commands are grouped, redirections may be applied to the entire command list. For example, the output of all the commands in the list may be redirected to a single stream.

()
( list )
Placing a list of commands between parentheses forces the shell to create a subshell (see Command Execution Environment), and each of the commands in list is executed in that subshell environment. Since the list is executed in a subshell, variable assignments do not remain in effect after the subshell completes.


The exit status of both of these constructs is the exit status of list.

3.4 Shell Parameters
A parameter is an entity that stores values. It can be a name, a number, or one of the special characters listed below. A variable is a parameter denoted by a name. A variable has a value and zero or more attributes. Attributes are assigned using the declare builtin command (see the description of the declare builtin in Bash Builtin Commands). The export and readonly builtins assign specific attributes.

A parameter is set if it has been assigned a value. The null string is a valid value. Once a variable is set, it may be unset only by using the unset builtin command.

A variable is assigned to using a statement of the form

name=[value]
If value is not given, the variable is assigned the null string. All values undergo tilde expansion, parameter and variable expansion, command substitution, arithmetic expansion, and quote removal (see Shell Parameter Expansion). If the variable has its integer attribute set, then value is evaluated as an arithmetic expression even if the $((…)) expansion is not used (see Arithmetic Expansion). Word splitting and filename expansion are not performed. Assignment statements may also appear as arguments to the alias, declare, typeset, export, readonly, and local builtin commands (declaration commands). When in POSIX mode (see Bash and POSIX), these builtins may appear in a command after one or more instances of the command builtin and retain these assignment statement properties. For example,

command export var=value
In the context where an assignment statement is assigning a value to a shell variable or array index (see Arrays), the ‘+=’ operator appends to or adds to the variable’s previous value. This includes arguments to declaration commands such as declare that accept assignment statements. When ‘+=’ is applied to a variable for which the integer attribute has been set, the variable’s current value and value are each evaluated as arithmetic expressions, and the sum of the results is assigned as the variable’s value. The current value is usually an integer constant, but may be an expression. When ‘+=’ is applied to an array variable using compound assignment (see Arrays), the variable’s value is not unset (as it is when using ‘=’), and new values are appended to the array beginning at one greater than the array’s maximum index (for indexed arrays), or added as additional key-value pairs in an associative array. When applied to a string-valued variable, value is expanded and appended to the variable’s value.

3.4.2 Special Parameters
The shell treats several parameters specially. These parameters may only be referenced; assignment to them is not allowed. Special parameters are denoted by one of the following characters.

*
($*) Expands to the positional parameters, starting from one. When the expansion is not within double quotes, each positional parameter expands to a separate word. In contexts where word expansions are performed, those words are subject to further word splitting and filename expansion. When the expansion occurs within double quotes, it expands to a single word with the value of each parameter separated by the first character of the IFS variable. That is, "$*" is equivalent to "$1c$2c…", where c is the first character of the value of the IFS variable. If IFS is unset, the parameters are separated by spaces. If IFS is null, the parameters are joined without intervening separators.

?
($?) Expands to the exit status of the most recently executed command.

$
($$) Expands to the process ID of the shell. In a subshell, it expands to the process ID of the invoking shell, not the subshell.
1
3.5 Shell Expansions
Expansion is performed on the command line after it has been split into tokens. Bash performs these expansions:

brace expansion
tilde expansion
parameter and variable expansion
command substitution
arithmetic expansion
word splitting
filename expansion
quote removal

The order of expansions is: brace expansion; tilde expansion, parameter and variable expansion, arithmetic expansion, and command substitution (done in a left-to-right fashion); word splitting; filename expansion; and quote removal.

//if quoted : echo "arg", arg is one token
//          : echo "arg1" "arg2", arg1 and arg2 are token, split by metachar <space>
//          : echo ""arg1" "arg2"", null, arg1<space>arg2, null. when it is passed to parser, null are not recognized.
//          : 

The ‘$’ character introduces parameter expansion, command substitution, or arithmetic expansion. The parameter name or symbol to be expanded may be enclosed in braces, which are optional but serve to protect the variable to be expanded from characters immediately following it which could be interpreted as part of the name. For example, if the first positional parameter has the value ‘a’, then ${11} expands to the value of the eleventh positional parameter, while $11 expands to ‘a1’.

3.5.4 Command Substitution
Command substitution allows the output of a command to replace the command itself. The standard form of command substitution occurs when a command is enclosed as follows:

$(command)

Bash performs command substitution by executing command in a subshell environment and replacing the command substitution with the standard output of the command, with any trailing newlines deleted. Embedded newlines are not deleted, but they may be removed during word splitting. The command substitution $(cat file) can be replaced by the equivalent but faster $(< file).



Quote removal is always performed last. It removes quote characters present in the original word, not ones resulting from one of the other expansions, unless they have been quoted themselves. See Quote Removal for more details.

Only brace expansion, word splitting, and filename expansion can increase the number of words of the expansion; other expansions expand a single word to a single word. The only exceptions to this are the expansions of "$@" and $* (see Special Parameters), and "${name[@]}" and ${name[*]} (see Arrays).


The ‘$’ character introduces parameter expansion, command substitution, or arithmetic expansion. The parameter name or symbol to be expanded may be enclosed in braces, which are optional but serve to protect the variable to be expanded from characters immediately following it which could be interpreted as part of the name. For example, if the first positional parameter has the value ‘a’, then ${11} expands to the value of the eleventh positional parameter, while $11 expands to ‘a1’.

When braces are used, the matching ending brace is the first ‘}’ not escaped by a backslash or within a quoted string, and not within an embedded arithmetic expansion, command substitution, or parameter expansion.


Command substitution allows the output of a command to replace the command itself. The standard form of command substitution occurs when a command is enclosed as follows:

$(command)



3.5.5 Arithmetic Expansion
Arithmetic expansion evaluates an arithmetic expression and substitutes the result. The format for arithmetic expansion is:

$(( expression ))
The expression undergoes the same expansions as if it were within double quotes, but unescaped double quote characters in expression are not treated specially and are removed. All tokens in the expression undergo parameter and variable expansion, command substitution, and quote removal. The result is treated as the arithmetic expression to be evaluated. Since the way Bash handles double quotes can potentially result in empty strings, arithmetic expansion treats those as expressions that evaluate to 0. Arithmetic expansions may be nested.

The evaluation is performed according to the rules listed below (see Shell Arithmetic). If the expression is invalid, Bash prints a message indicating failure to the standard error, does not perform the substitution, and does not execute the command associated with the expansion.

Next: Word Splitting, Previous: Arithmetic Expansion, Up: Shell Expansions   [Contents][Index]

3.5.6 Process Substitution
Process substitution allows a process’s input or output to be referred to using a filename. It takes the form of

<(list)
or

>(list)
The process list is run asynchronously, and its input or output appears as a filename. This filename is passed as an argument to the current command as the result of the expansion.

If the >(list) form is used, writing to the file provides input for list. If the <(list) form is used, reading the file obtains the output of list. Note that no space may appear between the < or > and the left parenthesis, otherwise the construct would be interpreted as a redirection.

Process substitution is supported on systems that support named pipes (FIFOs) or the /dev/fd method of naming open files.

When available, process substitution is performed simultaneously with parameter and variable expansion, command substitution, and arithmetic expansion.

3.5.7 Word Splitting
The shell scans the results of parameter expansion, command substitution, and arithmetic expansion that did not occur within double quotes for word splitting. Words that were not expanded are not split.

The shell treats each character of $IFS as a delimiter, and splits the results of the other expansions into fields using these characters as field terminators.

An IFS whitespace character is whitespace as defined above (see Definitions) that appears in the value of IFS. Space, tab, and newline are always considered IFS whitespace, even if they don’t appear in the locale’s space category.

If IFS is unset, word splitting behaves as if its value were <space><tab><newline>, and treats these characters as IFS whitespace. If the value of IFS is null, no word splitting occurs, but implicit null arguments (see below) are still removed.

Word splitting begins by removing sequences of IFS whitespace characters from the beginning and end of the results of the previous expansions, then splits the remaining words.

If the value of IFS consists solely of IFS whitespace, any sequence of IFS whitespace characters delimits a field, so a field consists of characters that are not unquoted IFS whitespace, and null fields result only from quoting.

If IFS contains a non-whitespace character, then any character in the value of IFS that is not IFS whitespace, along with any adjacent IFS whitespace characters, delimits a field. This means that adjacent non-IFS-whitespace delimiters produce a null field. A sequence of IFS whitespace characters also delimits a field.

Explicit null arguments ("" or '') are retained and passed to commands as empty strings. Unquoted implicit null arguments, resulting from the expansion of parameters that have no values, are removed. Expanding a parameter with no value within double quotes produces a null field, which is retained and passed to a command as an empty string.

When a quoted null argument appears as part of a word whose expansion is non-null, word splitting removes the null argument portion, leaving the non-null expansion. That is, the word -d'' becomes -d after word splitting and null argument removal.

Next: Quote Removal, Previous: Word Splitting, Up: Shell Expansions   [Contents][Index]

3.5.8 Filename Expansion
After word splitting, unless the -f option has been set (see The Set Builtin), Bash scans each word for the characters ‘*’, ‘?’, and ‘[’. If one of these characters appears, and is not quoted, then the word is regarded as a pattern, and replaced with a sorted list of filenames matching the pattern (see Pattern Matching), subject to the value of the GLOBSORT shell variable (see Bash Variables).

If no matching filenames are found, and the shell option nullglob is disabled, the word is left unchanged. If the nullglob option is set, and no matches are found, the word is removed. If the failglob shell option is set, and no matches are found, Bash prints an error message and does not execute the command. If the shell option nocaseglob is enabled, the match is performed without regard to the case of alphabetic characters.

When a pattern is used for filename expansion, the character ‘.’ at the start of a filename or immediately following a slash must be matched explicitly, unless the shell option dotglob is set. In order to match the filenames . and .., the pattern must begin with ‘.’ (for example, ‘.?’), even if dotglob is set. If the globskipdots shell option is enabled, the filenames . and .. never match, even if the pattern begins with a ‘.’. When not matching filenames, the ‘.’ character is not treated specially.

When matching a filename, the slash character must always be matched explicitly by a slash in the pattern, but in other matching contexts it can be matched by a special pattern character as described below (see Pattern Matching).

See the description of shopt in The Shopt Builtin, for a description of the nocaseglob, nullglob, globskipdots, failglob, and dotglob options.

The GLOBIGNORE shell variable may be used to restrict the set of file names matching a pattern. If GLOBIGNORE is set, each matching file name that also matches one of the patterns in GLOBIGNORE is removed from the list of matches. If the nocaseglob option is set, the matching against the patterns in GLOBIGNORE is performed without regard to case. The filenames . and .. are always ignored when GLOBIGNORE is set and not null. However, setting GLOBIGNORE to a non-null value has the effect of enabling the dotglob shell option, so all other filenames beginning with a ‘.’ match. To get the old behavior of ignoring filenames beginning with a ‘.’, make ‘.*’ one of the patterns in GLOBIGNORE. The dotglob option is disabled when GLOBIGNORE is unset. The GLOBIGNORE pattern matching honors the setting of the extglob shell option.


3.5.8.1 Pattern Matching
Any character that appears in a pattern, other than the special pattern characters described below, matches itself. The NUL character may not occur in a pattern. A backslash escapes the following character; the escaping backslash is discarded when matching. The special pattern characters must be quoted if they are to be matched literally.

Matches any string, including the null string. When the globstar shell option is enabled, and ‘*’ is used in a filename expansion context, two adjacent ‘*’s used as a single pattern match all files and zero or more directories and subdirectories. If followed by a ‘/’, two adjacent ‘*’s match only directories and subdirectories.


3.5.9 Quote Removal
After the preceding expansions, all unquoted occurrences of the characters ‘\’, ‘'’, and ‘"’ that did not result from one of the above expansions are removed.






3.6 Redirections
Before a command is executed, its input and output may be redirected using a special notation interpreted by the shell. Redirection allows commands’ file handles to be duplicated, opened, closed, made to refer to different files, and can change the files the command reads from and writes to. When used with the exec builtin, redirections modify file handles in the current shell execution environment. The following redirection operators may precede or appear anywhere within a simple command or may follow a command. Redirections are processed in the order they appear, from left to right.

Each redirection that may be preceded by a file descriptor number may instead be preceded by a word of the form {varname}. In this case, for each redirection operator except >&- and <&-, the shell allocates a file descriptor greater than or equal to 10 and assigns it to {varname}. If {varname} precedes >&- or <&-, the value of varname defines the file descriptor to close. If {varname} is supplied, the redirection persists beyond the scope of the command, which allows the shell programmer to manage the file descriptor’s lifetime manually without using the exec builtin. The varredir_close shell option manages this behavior (see The Shopt Builtin).

In the following descriptions, if the file descriptor number is omitted, and the first character of the redirection operator is ‘<’, the redirection refers to the standard input (file descriptor 0). If the first character of the redirection operator is ‘>’, the redirection refers to the standard output (file descriptor 1).

The word following the redirection operator in the following descriptions, unless otherwise noted, is subjected to brace expansion, tilde expansion, parameter and variable expansion, command substitution, arithmetic expansion, quote removal, filename expansion, and word splitting. If it expands to more than one word, Bash reports an error.

The order of redirections is significant. For example, the command

ls > dirlist 2>&1
directs both standard output (file descriptor 1) and standard error (file descriptor 2) to the file dirlist, while the command

ls 2>&1 > dirlist
directs only the standard output to file dirlist, because the standard error was made a copy of the standard output before the standard output was redirected to dirlist.

Bash handles several filenames specially when they are used in redirections, as described in the following table. If the operating system on which Bash is running provides these special files, Bash uses them; otherwise it emulates them internally with the behavior described below.

/dev/fd/fd
If fd is a valid integer, duplicate file descriptor fd.

/dev/stdin
File descriptor 0 is duplicated.

/dev/stdout
File descriptor 1 is duplicated.

/dev/stderr
File descriptor 2 is duplicated.

/dev/tcp/host/port
If host is a valid hostname or Internet address, and port is an integer port number or service name, Bash attempts to open the corresponding TCP socket.

/dev/udp/host/port
If host is a valid hostname or Internet address, and port is an integer port number or service name, Bash attempts to open the corresponding UDP socket.

A failure to open or create a file causes the redirection to fail.

Redirections using file descriptors greater than 9 should be used with care, as they may conflict with file descriptors the shell uses internally.

Redirecting Input
Redirecting Output
Appending Redirected Output
Redirecting Standard Output and Standard Error
Appending Standard Output and Standard Error
Here Documents
Here Strings
Duplicating File Descriptors
Moving File Descriptors
Opening File Descriptors for Reading and Writing


3.6.1 Redirecting Input
Redirecting input opens the file whose name results from the expansion of word for reading on file descriptor n, or the standard input (file descriptor 0) if n is not specified.

The general format for redirecting input is:

[n]<word
3.6.2 Redirecting Output
Redirecting output opens the file whose name results from the expansion of word for writing on file descriptor n, or the standard output (file descriptor 1) if n is not specified. If the file does not exist it is created; if it does exist it is truncated to zero size.

The general format for redirecting output is:

[n]>[|]word
If the redirection operator is ‘>’, and the noclobber option to the set builtin command has been enabled, the redirection fails if the file whose name results from the expansion of word exists and is a regular file. If the redirection operator is ‘>|’, or the redirection operator is ‘>’ and the noclobber option to the set builtin is not enabled, Bash attempts the redirection even if the file named by word exists.

3.6.3 Appending Redirected Output
Redirecting output in this fashion opens the file whose name results from the expansion of word for appending on file descriptor n, or the standard output (file descriptor 1) if n is not specified. If the file does not exist it is created.

The general format for appending output is:

[n]>>word
3.6.4 Redirecting Standard Output and Standard Error
This construct redirects both the standard output (file descriptor 1) and the standard error output (file descriptor 2) to the file whose name is the expansion of word.

There are two formats for redirecting standard output and standard error:

&>word
and

>&word
Of the two forms, the first is preferred. This is semantically equivalent to

>word 2>&1
When using the second form, word may not expand to a number or ‘-’. If it does, other redirection operators apply (see Duplicating File Descriptors below) for compatibility reasons.

3.6.5 Appending Standard Output and Standard Error
This construct appends both the standard output (file descriptor 1) and the standard error output (file descriptor 2) to the file whose name is the expansion of word.

The format for appending standard output and standard error is:

&>>word
This is semantically equivalent to

>>word 2>&1
(see Duplicating File Descriptors below).

3.6.6 Here Documents
This type of redirection instructs the shell to read input from the current source until it reads a line containing only delimiter (with no trailing blanks). All of the lines read up to that point then become the standard input (or file descriptor n if n is specified) for a command.

The format of here-documents is:

[n]<<[−]word
        here-document
delimiter
The shell does not perform parameter and variable expansion, command substitution, arithmetic expansion, or filename expansion on word.

If any part of word is quoted, the delimiter is the result of quote removal on word, and the lines in the here-document are not expanded. If word is unquoted, delimiter is word itself, and the here-document text is treated similarly to a double-quoted string: all lines of the here-document are subjected to parameter expansion, command substitution, and arithmetic expansion, the character sequence \newline is treated literally, and ‘\’ must be used to quote the characters ‘\’, ‘$’, and ‘`’; however, double quote characters have no special meaning.

If the redirection operator is ‘<<-’, the shell strips leading tab characters are stripped from input lines and the line containing delimiter. This allows here-documents within shell scripts to be indented in a natural fashion.

If the delimiter is not quoted, the \<newline> sequence is treated as a line continuation: the two lines are joined and the backslash-newline is removed. This happens while reading the here-document, before the check for the ending delimiter, so joined lines can form the end delimiter.


3.6.10 Opening File Descriptors for Reading and Writing
The redirection operator
