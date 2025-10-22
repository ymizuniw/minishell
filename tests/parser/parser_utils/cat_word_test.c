#include "../../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void	test_cat_word(void)
{
	char	*word;
	char	entry;
	size_t	entry_len;

	word = malloc(100);
	strcpy(word, "hello");
	entry = 'x';
	entry_len = 5;
	cat_word(&word, entry, entry_len);
	printf("cat_word test completed\n");
	printf("Result word: %s\n", word);
	free(word);
}

int	main(void)
{
	test_cat_word();
	return (0);
}
