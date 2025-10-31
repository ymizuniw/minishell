// Minimal get_pwd implementation returning current working directory
#include "../../../includes/minishell.h"

char	*get_pwd(void)
{
	return (getcwd(NULL, 0));
}
