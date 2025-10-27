
#include "../../../../includes/minishell.h"

char	*ext_unit(char *src, size_t start, size_t end)
{
	char	*unit;

	size_t len = end - start;
	unit = xmalloc(sizeof(char) * len);
	strncpy(unit, &src[start], len);
	unit[len] = '\0';
	return (unit);
}

int	join_value(char **res, const char *value, size_t size1, size_t size2)
{
	*res = realloc(*res, sizeof(char) * (size1 + size2 + 1));
	if (*res == NULL)
		return (0);
	strncpy(*res + size1 + 1, value, size2);
	(*res)[size1 + size2] = '\0';
	return (1);
}

//improved ver heredoc_value_expansion.
char	*heredoc_value_expansion(const char *line, bool in_quote, size_t len)
{
	size_t	i = 0;
	char	*res = strdup("");
	while (i < len)
	{
		if (line[i] == '$' && !in_quote)
		{
			size_t start = ++i;
			while (i < len && (isalnum(line[i]) || line[i] == '_'))
				i++;
			char *varname = ext_unit((char *)line, start, i);
			const char *val = getenv(varname);
			if (!val) val = "";
			join_value(&res, val, strlen(res), strlen(val));
			free(varname);
		}
		else
		{
			char tmp[2] = {line[i++], '\0'};
			join_value(&res, tmp, strlen(res), 1);
		}
	}
	return res;
}

char	*heredoc_expansion(char *line, bool in_quote, size_t line_len)
{
	char	*new;

	new = heredoc_value_expansion(line, in_quote, line_len);
	if (new == NULL)
		return (NULL);
	return (new);
}

int	get_document(t_redir *hd, char **document, size_t *document_len)
{
	char	*delim;
	char	*line;
	char	*value;

	delim = hd->filename;
	line = NULL;
	while (1)
	{
		line = readline("> ");
		if (strcmp(line, delim) == 0)
			return (1);
		if (!line)
			break;
		if (strcmp(line, delim) == 0)
			break;
		value = heredoc_expansion(line, hd->delim_quoted, strlen(line));
		if (!value || !join_value(document, value, *document_len, strlen(value)))
		{
			xfree(line);
			xfree(value);
			return (-1);
		}
		*document_len += strlen(value);
	}
	return (-1);
}

ssize_t	heredoc_write_to_fd(int herepipe[2], char *document,
		size_t document_len)
{
	ssize_t	wb;

	wb = 0;
	wb = write(herepipe[1], document, document_len);
	if (wb != (ssize_t)document_len)
		return (0);
	return (wb);
}

int	get_tmp_fd(char *src, size_t size, char **filename)
{
	int		tmp_fd;
	ssize_t	wb;

	tmp_fd = ft_mkstmpfd(HERE_TEMPLATE, filename);
	if (tmp_fd < 0)
		return (-1);
	wb = write(tmp_fd, src, size);
	if (wb != (ssize_t)size)
	{
		close(tmp_fd);
		return (-1);
	}
	return (tmp_fd);
}

//refined version
int	make_heredoc(t_redir *hd)
{
	char	*document = NULL;
	size_t	document_len = 0;
	char	*filename = NULL;
	int		fd, tmp_fd, herepipe[2];

	if (get_document(hd, &document, &document_len) < 0)
		return (-1);
	if (document_len == 0)
	{
		fd = open("/dev/null", O_RDONLY);
		xfree(document);
		return (fd);
	}
	if (document_len + 1 <= HERE_PIPE_SIZE)
	{
		if (pipe(herepipe) < 0)
			return (perror("pipe"), free(document), -1);
		ssize_t wb = write(herepipe[1], document, document_len);
		close(herepipe[1]);
		free(document);
		if (wb != (ssize_t)document_len)
			return (close(herepipe[0]), -1);
		return (herepipe[0]);
	}
	else
	{
		tmp_fd = get_tmp_fd(document, document_len, &filename);
		free(document);
		if (tmp_fd < 0)
			return (-1);
		close(tmp_fd);
		fd = open(filename, O_RDONLY);
		free(filename);
		return (fd);
	}
}
