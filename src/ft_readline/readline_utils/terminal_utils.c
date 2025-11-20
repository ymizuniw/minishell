/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:38:41 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/21 01:21:43 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	enable_raw_mode(struct termios *orig)
{
	struct termios	raw;

	if (orig == NULL)
		printf("orig is null\n");
	tcgetattr(STDIN_FILENO, orig);
	raw = *orig;
	raw.c_lflag &= ~(ICANON | ECHO);
	raw.c_cc[VMIN] = 1;
	raw.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

void	disable_raw_mode(const struct termios *orig)
{
	tcsetattr(STDIN_FILENO, TCSANOW, orig);
}

ssize_t	read_key(char *c)
{
	return (read(STDIN_FILENO, c, 1));
}

void	redraw_line(const char *prompt, const char *buf)
{
	size_t	prompt_len;
	size_t	buf_len;

	write(STDOUT_FILENO, "\r\x1b[K", 4);
	prompt_len = 0;
	if (prompt)
		prompt_len = ft_strlen(prompt);
	write(STDOUT_FILENO, prompt, prompt_len);
	buf_len = 0;
	if (buf)
		buf_len = ft_strlen(buf);
	write(STDOUT_FILENO, buf, buf_len);
}
