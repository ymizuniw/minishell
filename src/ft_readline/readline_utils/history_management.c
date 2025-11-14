#include "../../../includes/minishell.h"

void	add_history(char *line, t_hist *hist)
{
	if (!line || !*line)
		return ;
	if (hist && hist->hist_box[hist->idx] != NULL)
		xfree(hist->hist_box[hist->idx]);
	hist->hist_box[hist->idx] = ft_strdup(line);
	hist->idx = (hist->idx + 1) % HIST_MAX;
	if (hist->size < HIST_MAX)
		hist->size++;
	hist->cur = -1;
}

void	free_hist_box(char *hist_box[HIST_MAX])
{
	size_t	i;

	i = 0;
	while (i < HIST_MAX)
	{
		xfree(hist_box[i]);
		hist_box[i] = NULL;
		i++;
	}
}
