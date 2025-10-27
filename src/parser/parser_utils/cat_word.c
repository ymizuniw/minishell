#include "../../../includes/minishell.h"

// concatenate word after $ expansion.
void	cat_word(char **word, char *entry, size_t entry_len)
{
	size_t	word_len;
	char	*new;

	word_len = 0;
	if (word && *word)
		word_len = strlen(*word);
	new = xmalloc(sizeof(char) * (word_len + entry_len + 1));
	if (new == NULL)
	{
		perror("malloc :");
		exit(1);
	}
	if (*word)
	{
		strncpy(new, *word, word_len + 1);
		xfree(*word);
	}
	else
		new[0] = '\0';
	strcpy(new + word_len, entry);
	*word = new;
}
