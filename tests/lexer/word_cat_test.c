#include "../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void	test_word_cat(void)
{
	char	*word;
	size_t	word_len;
	char	*input;
	size_t	start_idx;
	size_t	end_idx;
	size_t	result;

	word = malloc(100);
	strcpy(word, "hello");
	word_len = strlen(word);
	input = " world";
	start_idx = 1;
	end_idx = 6;
	result = word_cat(&word, word_len, input, start_idx, end_idx);
	printf("word_cat test result: %zu\n", result);
	printf("Concatenated word: %s\n", word);
	free(word);
}

int	main(void)
{
	test_word_cat();
	return (0);
}
