#include "../../../../includes/minishell.h"

char	**expand_wildcard_word(t_word *word, char **res, size_t *i,
		size_t *word_list_count)
{
	char	**wildcard_arr;
	char	**new_res;
	size_t	wildcard_count;
	size_t	wildcard_idx;

	wildcard_count = 0;
	wildcard_arr = expand_wildcard(word->word, "./", &wildcard_count);
	if (!wildcard_arr)
		return (free_double_array(res), NULL);
	new_res = realloc(res, sizeof(char *) * (*word_list_count + wildcard_count
				+ 1));
	if (!new_res)
		return (free_double_array(res), free_double_array(wildcard_arr), NULL);
	res = new_res;
	wildcard_idx = 0;
	while (wildcard_idx < wildcard_count)
	{
		res[*i] = strdup(wildcard_arr[wildcard_idx]);
		if (!res[*i])
			return (free_double_array(res), free_double_array(wildcard_arr),
				NULL);
		(*i)++;
		wildcard_idx++;
	}
	*word_list_count += wildcard_count;
	free_double_array(wildcard_arr);
	return (res);
}
