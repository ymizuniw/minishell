/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_printable.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 17:20:04 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/03 04:53:23 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_readline.h"

int	process_printable(t_new_readline *nr, char one_char)
{
	int	ret;

	if (is_inserting(nr))
		ret = insertion(nr, one_char);
	else
		ret = write_to_buffer(nr, one_char);
	redraw_line(nr);
	return (ret);
}
