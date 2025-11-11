#include "../../../../includes/minishell.h"

// Return value should not be freed separately
char	*expand_doller(t_word *word, t_shell *shell)
{
	char	*env_value;
	char	*target;
	char	exit_status_str[12];

	env_value = NULL;
	target = word->word;
	if (word->type == WD_DOLLER)
		target = &word->word[1];
	// Handle special variable $?
	if (strcmp(target, "?") == 0)
	{
		snprintf(exit_status_str, sizeof(exit_status_str), "%d",
			shell->last_exit_status);
		return (strdup(exit_status_str));
	}
	env_value = get_env_value(shell->env_list, target);
	if (!env_value)
		return (strdup(""));
	return (strdup(env_value));
}

char	*expand_dollar_word(t_word *word, t_shell *shell)
{
	char	*env_expand;

	env_expand = expand_doller(word, shell);
	if (!env_expand)
		env_expand = strdup("");
	return (env_expand);
}
