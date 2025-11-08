#include "../../../../includes/minishell.h"

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

int	join_value(char **res, const char *value, size_t size1, size_t size2)
{
	*res = realloc(*res, sizeof(char) * (size1 + size2 + 1));
	if (*res == NULL)
		return (0);
	strncpy(*res + size1, value, size2);
	(*res)[size1 + size2] = '\0';
	return (1);
}

static char	*expand_exit_status(char **res, t_shell *shell, size_t *i)
{
	char	*s;

	s = ft_itoa(shell->last_exit_status);
	if (!s)
		return (free(*res), NULL);
	if (!join_value(res, s, strlen(*res), strlen(s)))
		return (free(s), free(*res), NULL);
	free(s);
	*i += 2;
	return (*res);
}

static char	*expand_variable(char **res, const char *line, size_t len,
		t_heredoc_ctx *ctx)
{
	size_t		start;
	char		*varname;
	const char	*val;
	t_env		*env_entry;

	start = ++ctx->i;
	while (ctx->i < len && (isalnum((unsigned char)line[ctx->i])
			|| line[ctx->i] == '_'))
		ctx->i++;
	varname = ext_unit((char *)line, start, ctx->i);
	if (!varname)
		return (free(*res), NULL);
	env_entry = find_env(ctx->shell->env_list, varname);
	val = env_entry && env_entry->value ? env_entry->value : "";
	if (!join_value(res, val, strlen(*res), strlen(val)))
		return (free(varname), free(*res), NULL);
	return (free(varname), *res);
}

static char	*heredoc_value_expansion(const char *line, bool in_quote,
		size_t len, t_shell *shell)
{
	char			*res;
	char			tmp[2];
	t_heredoc_ctx	ctx;

	ctx = (t_heredoc_ctx){0, shell};
	res = strdup("");
	if (!res)
		return (NULL);
	while (ctx.i < len)
	{
		if (line[ctx.i] == '$' && !in_quote)
		{
			if (ctx.i + 1 <= len && line[ctx.i + 1] == '?')
			{
				if (!expand_exit_status(&res, shell, &ctx.i))
					return (NULL);
				continue ;
			}
			if (!expand_variable(&res, line, len, &ctx))
				return (NULL);
		}
		else
		{
			tmp[0] = line[ctx.i++];
			tmp[1] = '\0';
			if (!join_value(&res, tmp, strlen(res), 1))
				return (free(res), NULL);
		}
	}
	return (res);
}

static int	process_heredoc_line(char **document, size_t *document_len,
		char *line, t_heredoc_params *params)
{
	char	*value;

	value = heredoc_value_expansion(line, params->delim_quoted, strlen(line),
			params->shell);
	if (!value || !join_value(document, value, *document_len, strlen(value)))
		return (xfree(line), xfree(value), -1);
	*document_len += strlen(value);
	if (!join_value(document, "\n", *document_len, 1))
		return (xfree(line), xfree(value), -1);
	*document_len += 1;
	return (free(line), free(value), 1);
}

int	get_document(t_redir *hd, char **document, size_t *document_len,
		t_shell *shell)
{
	char				*line;
	t_heredoc_params	params;

	params = (t_heredoc_params){hd->filename, hd->delim_quoted, shell};
	line = NULL;
	while (1)
	{
		line = ft_readline(">", NULL);
		if (!line)
			break ;
		if (strcmp(line, params.delim) == 0)
			return (free(line), 1);
		if (process_heredoc_line(document, document_len, line, &params) < 0)
			return (-1);
	}
	return (-1);
}

static int	make_pipe_heredoc(char *document, size_t document_len)
{
	int		herepipe[2];
	ssize_t	wb;

	if (pipe(herepipe) < 0)
		return (perror("pipe"), free(document), -1);
	wb = write(herepipe[1], document, document_len);
	close(herepipe[1]);
	free(document);
	if (wb != (ssize_t)document_len)
		return (close(herepipe[0]), -1);
	return (herepipe[0]);
}

static int	make_file_heredoc(char *document)
{
	char	*filename;
	int		tmp_fd;
	int		fd;

	filename = NULL;
	tmp_fd = ft_mkstmpfd("tmp/", &filename);
	free(document);
	if (tmp_fd < 0)
		return (-1);
	close(tmp_fd);
	fd = open(filename, O_RDONLY);
	free(filename);
	return (fd);
}

int	make_heredoc(t_redir *hd, t_shell *shell)
{
	char	*document;
	size_t	document_len;
	int		fd;

	document = NULL;
	document_len = 0;
	if (get_document(hd, &document, &document_len, shell) < 0)
		return (xfree(document), -1);
	if (document_len == 0)
	{
		fd = open("/dev/null", O_RDONLY);
		xfree(document);
		return (fd);
	}
	if (document_len + 1 <= HERE_PIPE_SIZE)
		return (make_pipe_heredoc(document, document_len));
	else
		return (make_file_heredoc(document));
}
