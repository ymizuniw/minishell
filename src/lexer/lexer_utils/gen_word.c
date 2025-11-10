#include "../../../includes/minishell.h"

char **ft_expand_word(t_word *word_list, t_env *env_list);
char *expand_doller(t_word *word, t_env *env_list);
char	**expand_wildcard(const char *pattern, const char *path, size_t *wildcard_count);

//inclibmimnishell
char	*ext_unit(char *src, size_t start, size_t end)
{
	char	*unit;
	size_t	len;

	len = end - start;
	unit = xmalloc(sizeof(char) * (len + 1));
	if (unit == NULL)
		return (NULL);
	strncpy(unit, &src[start], len);
	unit[len] = '\0';
	return (unit);
}

//inc libminishell
int	join_value(char **res, const char *value, size_t size1, size_t size2)
{
	*res = realloc(*res, sizeof(char) * (size1 + size2 + 1));
	if (*res == NULL)
		return (0);
	strncpy(*res + size1, value, size2);
	(*res)[size1 + size2] = '\0';
	return (1);
}

static size_t count_word_list(t_word *word)
{
    size_t size=0;
    
    while(word)
    {
        size++;
        word = word->next;
    }
    return (size);
}

//maybe the return value of this function shall not be free()ed not in the scope for free() the env_list. 
char *expand_doller(t_word *word, t_env *env_list)
{
    char *env_value=NULL;
    char *target = word->word;

    if (word->type==WD_DOLLER)
        target = &word->word[1];
    env_value = get_env_value(env_list, target);
    if (!env_value)
        env_value = "";
    return (env_value);
}

char **ft_expand_word(t_word *wd_list, t_env *env_list)
{
	t_word *word = wd_list;
	char *plain_expand = NULL;
	char *env_expand = NULL;
	char **wildcard_arr = NULL;
	size_t wildcard_count = 0;
	size_t wildcard_idx = 0;
	char **res = NULL;
	size_t word_list_count = 0;
	size_t i = 0;

	word_list_count = count_word_list(wd_list);
	res = (char **)xcalloc(sizeof(char *) * (word_list_count + 1));
	if (!res)
		return (NULL);

	while (i < word_list_count && word)
	{
		if (word->to_expand_doller)
		{
			env_expand = expand_doller(word, env_list);
			if (!env_expand)
				env_expand = "";
			res[i] = strdup(env_expand);
			if (!res[i])
				return (free_double_array(res), NULL);
			i++;
		}
		else if (word->to_expand_wildcard)
		{
			wildcard_arr = expand_wildcard(word->word, "./", &wildcard_count);
			if (!wildcard_arr)
				return (free_double_array(res), NULL);
			res = realloc(res, sizeof(char *) * (word_list_count + wildcard_count + 1));
			if (!res)
				return (free_double_array(wildcard_arr), NULL);

			for (wildcard_idx = 0; wildcard_idx < wildcard_count; wildcard_idx++)
			{
				res[i] = strdup(wildcard_arr[wildcard_idx]);
				if (!res[i])
					return (free_double_array(res), free_double_array(wildcard_arr), NULL);
				i++;
			}
			word_list_count += wildcard_count;
			free_double_array(wildcard_arr);
			wildcard_arr = NULL;
		}
		else
		{
			assert(word->word != NULL);
			plain_expand = strdup(word->word);
			if (!plain_expand)
				return (free_double_array(res), NULL);
			res[i] = plain_expand;
			i++;
		}
		word = word->next;
	}
	res[i] = NULL;
	return (res);
}

// //argv=ft_expand_word();
// char  **ft_expand_word(t_word *wd_list, t_env *env_list)
// {
//     t_word *word = wd_list;
//     char *plain_expand = NULL;
//     char *env_expand = NULL;
//     char **wildcard_arr = NULL;
//     size_t wildcard_count=0;
//     size_t wildcard_idx = 0;
//     char **res = NULL;
//     size_t word_list_count = 0;
//     size_t i = 0;

//     word_list_count = count_word_list(wd_list);
//     res = (char **)xcalloc(sizeof(char *)*(word_list_count+1));
//     if (res==NULL)
//         return (NULL);
//     while (i<word_list_count)
//     {
//         if (word->to_expand_doller || word->to_expand_wildcard)
//         {
//             if (word->to_expand_doller)
//             {
//                 env_expand = expand_doller(word, env_list);
//                 res[i] = env_expand;
//                 i++;
//             }
//             else if (word->to_expand_wildcard)
//             {
//                 wildcard_arr = expand_wildcard(word->word, "./", &wildcard_count);
//                 if (wildcard_arr==NULL)
//                     return(NULL);
//                 word_list_count += wildcard_count;
//                 //loop till the wildcard_arr element null.
//                 //count size here or delegate ptr of size_t wildcard_arr_size and internally count it. eff.
//                 res = realloc(res, word_list_count + 1);
//                 if (res==NULL)
//                 {
//                     free_double_array(wildcard_arr);
//                     return(NULL);
//                 }
//                 while(wildcard_idx<wildcard_count)
//                 {
//                     size_t res_i_len=0;

//                     if (res[i])
//                     {
//                         // if (i<3)
//                             // printf("res[%zu]: %s\n", i, res[i]);
//                         // printf("i: %zu\n res address:%p\n res[%zu]: %s\n", i, res[i], i , res[i]);
//                         // printf("%zu\n", i);
//                         res_i_len = strlen(res[i]);
//                     }
//                     size_t value_len=0;
//                     if (wildcard_arr[wildcard_idx])
//                         value_len = strlen(wildcard_arr[wildcard_idx]);
//                     if (!join_value(&res[i], wildcard_arr[wildcard_idx], res_i_len, value_len))
//                     {
//                         free_double_array(res);
//                         free_double_array(wildcard_arr);
//                         return (NULL);
//                     }
//                     i++;
//                     wildcard_idx++;
//                 }
//             }
//         }
//         else
//         {
//             size_t res_i_len=0;
//             size_t plain_expand_len = 0;

//             assert(word->word!=NULL);
//             plain_expand = strdup(word->word);//this shall be free() ed, but expand_env() result shall not be, then flag is needed?
//             if (plain_expand==NULL)
//                 return (xfree(res), NULL);
//             if (res[i])
//                 res_i_len = strlen(res[i]);
//             if (plain_expand)
//                 plain_expand_len = strlen(plain_expand);
//             if (join_value(&res[i], plain_expand, res_i_len, plain_expand_len)<0)
//             {
//                 free_double_array(res);
//                 xfree(plain_expand);
//                 return (NULL);
//             }
//             xfree(plain_expand);
//             i++;
//         }
//         word = word->next;
//     }
//     free_double_array(wildcard_arr);
//     return (res);
// }

t_word *append_node(t_word *head, t_word *new)
{
    if (head==NULL)
        return (new);
    while (head->next)
        head=head->next;
    head->next = new;
    return (head);
}

size_t quote_close_place(char quote, char *value, size_t i)
{
    if (value==NULL)
        return (0);
    while (value[i]!='\0')
    {
        if (value[i]==quote)
            return (i);
        i++;
    }
    return (0);
}

//after split by metachar?
t_word *gen_word(char *value, size_t value_len, size_t *addition)
{
    size_t i = 0;
    char quote = is_quote(value[i]);
    size_t start = i;
    t_word *head = NULL;

    while (i<value_len)
    {
        t_word *word=(t_word *)xcalloc(sizeof(t_word));
        if (word==NULL)
            return NULL;
        word->word = strdup("");
        if (word->word==NULL)
        {
            free_word_list(head);
            xfree(word);
            return (NULL);
        }
        size_t close_place = 0;
        if (quote!='\0')
            close_place = quote_close_place(quote, value, ++i);
        if(quote!='\0' && close_place>0)
        {
            if (quote=='\'')
                word->to_expand_doller=false;
            else
                word->to_expand_doller=true;
            word->to_expand_wildcard=false;
            word->word = ext_unit(value, start, close_place);
            if (!word->word)
                return (free_word_list(head), xfree(word), NULL); 
            i = close_place+1;
        }
        else
        {
            if (value[i]=='$')
            {
                word->type=WD_DOLLER;
                word->to_expand_doller=true;
                i++;
            }
            else
                word->type=WD_LITERAL;
            while (i<value_len && !is_quote(value[i]) && value[i]!='$')
            {
                if(word->type==WD_DOLLER && value[i]=='*')
                    break;
                if (word->type!=WD_DOLLER && value[i]=='*')
                    word->to_expand_wildcard=true;
                i++;
            }
            word->word = ext_unit(value, start, i);
            if (!word->word)
                return (free_word_list(head), xfree(word), NULL); 
        }
        head = append_node(head, word);
    }
    *addition = i;
    return (head);
}
