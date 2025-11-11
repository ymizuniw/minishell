#include "../../../../includes/minishell.h"

static char	*expand_exit_status(char **res, t_shell *shell, size_t *i)
{
	char	*s;
	size_t	res_len;
	size_t	s_len;

	s = ft_itoa(shell->last_exit_status);
	if (!s)
		return (xfree(*res), NULL);
	res_len = 0;
	s_len = 0;
	if (*res)
		res_len = strlen(*res);
	if (s)
		s_len = strlen(s);
	if (!join_value(res, s, res_len, s_len))
		return (xfree(s), xfree(*res), NULL);
	xfree(s);
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
	size_t		res_len;
	size_t		val_len;

	start = ++ctx->i;
	while (ctx->i < len && (isalnum((unsigned char)line[ctx->i])
			|| line[ctx->i] == '_'))
		ctx->i++;
	varname = ext_unit((char *)line, start, ctx->i);
	if (!varname)
		return (xfree(*res), NULL);
	env_entry = find_env(ctx->shell->env_list, varname);
	if (env_entry && env_entry->value)
		val = env_entry->value;
	else
		val = "";
	res_len = 0;
	val_len = 0;
	if (res)
		res_len = strlen(*res);
	if (val)
		val_len = strlen(val);
	if (!join_value(res, val, res_len, val_len))
		return (xfree(varname), xfree(*res), NULL);
	return (xfree(varname), *res);
}

static char	*heredoc_value_expansion(const char *line, bool in_quote,
		size_t len, t_shell *shell)
{
	char			*res;
	char			tmp[2];
	t_heredoc_ctx	ctx;
	size_t			res_len;

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
			res_len = 0;
			if (res)
				res_len = strlen(res);
			if (!join_value(&res, tmp, res_len, 1))
				return (xfree(res), NULL);
		}
	}
	return (res);
}

static int	process_heredoc_line(char **document, size_t *document_len,
		char *line, t_heredoc_params *params)
{
	char	*value;
	size_t	line_len;
	size_t	value_len;

	line_len = 0;
	if (line)
		line_len = strlen(line);
	value = heredoc_value_expansion(line, params->delim_quoted, line_len,
			params->shell);
	if (!value)
		return (free(line), -1);
	value_len = strlen(value);
	if (!join_value(document, value, *document_len, value_len))
		return (free(line), free(value), -1);
	*document_len += value_len;
	if (!join_value(document, "\n", *document_len, 1))
		return (free(line), free(value), -1);
	*document_len += 1;
	return (free(line), free(value), 1);
}

int	get_document(t_redir *hd, char **document, size_t *document_len,
		t_shell *shell)
{
	char				*line;
	char				*line_copy;
	t_heredoc_params	params;
	size_t				len;
	int					is_interactive;
	size_t				current_len;

	params = (t_heredoc_params){hd->filename, hd->delim_quoted, shell};
	line = NULL;
	len = 0;
	is_interactive = isatty(STDIN_FILENO);
	while (1)
	{
		// Interactive mode: use ft_readline
		if (is_interactive)
		{
			line = ft_readline(">", NULL);
			if (!line)
				break ;
			line_copy = line;
		}
		// Non-interactive mode: read from stdin directly
		else
		{
			if (getline(&line, &len, stdin) == -1)
			{
				if (line)
					free(line);
				break ;
			}
			// Remove trailing newline from getline
			current_len = strlen(line);
			if (current_len > 0 && line[current_len - 1] == '\n')
				line[current_len - 1] = '\0';
			// Make a copy for processing (getline buffer is reused)
			line_copy = strdup(line);
			if (!line_copy)
			{
				free(line);
				return (-1);
			}
		}
		if (strcmp(line_copy, params.delim) == 0)
		{
			free(line_copy);
			if (!is_interactive)
				free(line);
			return (1);
		}
		if (process_heredoc_line(document, document_len, line_copy,
				&params) < 0)
		{
			if (!is_interactive)
				free(line);
			return (-1);
		}
	}
	if (!is_interactive && line)
		free(line);
	return (-1);
}

static int	make_pipe_heredoc(char *document, size_t document_len)
{
	int		herepipe[2];
	ssize_t	wb;

	if (pipe(herepipe) < 0)
		return (perror("pipe"), xfree(document), -1);
	wb = write(herepipe[1], document, document_len);
	close(herepipe[1]);
	xfree(document);
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
	xfree(document);
	if (tmp_fd < 0)
		return (-1);
	close(tmp_fd);
	fd = open(filename, O_RDONLY);
	xfree(filename);
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
