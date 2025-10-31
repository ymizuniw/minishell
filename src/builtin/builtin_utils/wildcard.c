// #include "../../../includes/minishell.h"

// typedef struct s_wildcard
// {
// 	char				*name[256];
// 	size_t				i;
// 	bool				expand;
// 	struct s_wildcard	*next;
// }						t_wildcard;

// // str = "str1*str2*"
// // >for each filename in s_wildcard object,  memmem() with str1, str2,
// // shifting the start point.

// int	append_wildcard(t_wildcard *head, t_wildcard *new)
// {
// 	if (head == NULL || new == NULL)
// 		return (0);
// 	while (head->next != NULL)
// 		head = head->next;
// 	head->next = new;
// 	return (1);
// }

// bool	match_to_keys(char **keys, char const *name)
// {
// 	char	*mem;
// 	char	*start;
// 	size_t	i;

// 	mem = NULL;
// 	start = name;
// 	i = 0;
// 	while (keys[i] != NULL)
// 	{
// 		mem = memmem(start, keys[i]);
// 		if (mem != NULL)
// 		{
// 			start = mem + strlen(keys[i]);
// 		}
// 		else
// 			return (false);
// 	}
// 	return (true);
// }

// int	mark_wc_to_expand(char const *str, t_wildcard *wc)
// {
// 	char	**keys;

// 	keys = NULL;
// 	keys = ft_split(str, '*');
// 	if (keys == NULL) // shall free_double_array();
// 		return (0);
// 	while (wc != NULL)
// 	{
// 		wc->expand = match_to_keys(keys, wc->name);
// 		wc = wc->next;
// 	}
// 	return (1);
// }

// int	expand_wildcard(char const *str, t_shell *shell)
// {
// 	t_wildcard		*head;
// 	t_wildcard		*new;
// 	DIR				*dirp;
// 	struct dirent	*dent;

// 	head = xmalloc(sizeof(t_wildcard));
// 	memset(head, 0, sizeof(t_wildcard));
// 	dirp = {0};
// 	dent = {0};
// 	dirp = opendir(shell->pwd);
// 	if (dirp == NULL)
// 	{
// 		perror("opendir: ");
// 		return (-1);
// 	}
// 	while (1)
// 	{
// 		dent = readdir(dirp);
// 		if (dent == NULL)
// 		{
// 			perror("readdir: ");
// 			return (-1);
// 		}
// 		new = xmalloc(sizeof(t_wildcard));
// 		new->expand = false;
// 		new->i = 0;
// 		strncpy(new->name, dent->d_name, strlen(dent->d_name));
// 		new->next = NULL;
// 		append_wildcard(head, new);
// 	}
// 	if (!mark_wc_to_expand(str, head))
// 		return (0);
// 	return (1);
// }

// // bash-3.2$ arg = "*"
// // bash: arg: command not found
// // bash-3.2$ arg="*"
// // bash-3.2$ echo $arg
// // documents includes Makefile src tests
// // bash-3.2$ echo ($arg)
// // bash: syntax error near unexpected token `$arg'
// // bash-3.2$ (echo $arg)
// // documents includes Makefile src tests
// // bash-3.2$ echo $arg

// // bash-3.2$ (echo $arg)
// // documents includes Makefile src tests
// // bash-3.2$ (echo $arg) && echo $arg
// // documents includes Makefile src tests
// // documents includes Makefile src tests
// // bash-3.2$ (export arg="tempo"; echo $arg) && echo $arg
// // tempo
// // documents includes Makefile src tests
// // bash-3.2$
