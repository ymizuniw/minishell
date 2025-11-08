#include "../../../includes/minishell.h"

bool	is_builtin(const char *cmd)
{
	if (!cmd)
		return (1);
	return (strcmp(cmd, "echo") == 0 || strcmp(cmd, "cd") == 0 || strcmp(cmd,
			"pwd") == 0 || strcmp(cmd, "export") == 0 || strcmp(cmd,
			"unset") == 0 || strcmp(cmd, "env") == 0 || strcmp(cmd,
			"exit") == 0);
}
