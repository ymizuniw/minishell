// #include "../../../../includes/minishell.h"

// int exec_path()
//{
//  set_env(env_list, external_command)//add external_command to env_list,
	// as follows: "_=absolute_path_of_command".
//  execve();
//  below here, execve() failed,
	// then remove previously set external_command from env_list.
//  unset_env(env_list, external_command)
//}

// int search_and_exec(char *cmd, char **env)
// {
//     /*Shell function is not required, maybe.
//         if no slash cmd name comes,attempts to locale it.
//         if there exists a shell function by that name,
//         that function is invoked as described in Shell Function.
//     */

//     /*
//         If the name does not match a function, the shell searches for it
//         in the list of shell builtins. If a match is found,
	// that builtin is invoked.
//     */
//     if (find_built_in())
//         return (exec_built_in());
//     /*
//         if the name is neither a shell function nor a builtin,
	// and contains no slashes,
//         Bash searches each element of $PATH for a directory containing an executable file by that name.
//         Bash uses a hash table to remmember the full pathnames of executable files to avoid multiple PATH searches.
//         Bash performs a full search of the directories in $PATH only if the command is not found in the hash table.
//         If the search is unsuccessfull,
	// the shell searches for a defined shell function named command_not_found_handle.
//         If that function exists,
	// it is invoked in a separate execution environment with the original command and the original command's
//         arguments as its arguments,
	// and the function's exit status becomes the exit status of that subshell.
//         If that function is not defined,
	// the shell prints an error message and returns an exit status of 127.
//     */
//     if (with_no_slash())
//     {
//        
	//int err = 0; the result of search_hash and find_path should be kept for return value,
	// maybe.
//         if (search_hash())//if you implement command hash map
//             return (exec_path());
//         else if (find_path())
//             return (exec_path());
//     }
//     /*
//         If the search is successful,
	// or if the command name contains one or more slashes,
//         the shell executes the named program in a separate execution environment. Argument 0
//         is set to the name given,
	// and the remaining arguments to the command are set to the arguments supplied,
	// if any.
//     */
//     else
//     {
//         return (exec_path());
//     }
//     /*
//         If this execution fails bacause the file is not in executable format,
	// and the file is not a directory,
//         it is assumed to be a shell script, a file containing shell commands,
	// and the shell executes it as described
//         in Shell Scripts.
//     */
//     /*
//         If the command was not begun asynchronously,
	// the shell waits for the command to complete and collects its exit status.
//     */
//     /*
//         Various process IDs,
	// including those of background jobs (see Lists of Commands), the value of $$,
	// and the value of $PPID.
//     */
// }

// //if cached in hash table and could not conducted,
// //it will return 127 or 126? not found or command failure?

// // Bash scripts often begin with #!
	// /bin/bash (assuming that Bash has been installed in /bin),
	// since this ensures that Bash will be used to interpret the script,
	// even if it is executed under another shell. It’s a common idiom to use env to find bash even if it’s been installed in another directory: #!/usr/bin/env bash will find the first occurrence of bash in $PATH.
