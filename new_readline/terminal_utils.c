/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:38:41 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/30 21:25:41 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_readline.h"

void	enable_raw_mode(struct termios *orig)
{
	struct termios	raw;

	tcgetattr(STDIN_FILENO, orig);
	raw = *orig;
	raw.c_lflag &= ~(ICANON | ECHO | ISIG);
	raw.c_cc[VMIN] = 1;
	raw.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

void	disable_raw_mode(const struct termios *orig)
{
	tcsetattr(STDIN_FILENO, TCSANOW, orig);
}
