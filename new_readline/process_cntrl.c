/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_cntrl.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 03:31:26 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/03 04:45:11 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_readline.h"
#include <termcap.h>

// static int ft_putchar(int c)
// {
//     write(1, &c, 1);
//     return (1);
// }

int process_cntrl(t_new_readline *nr, char one_char)
{
    (void)nr;
    if (one_char == 0x0C)
    {
        // tputs(tgetstr("cl", NULL), 1, ft_putchar);
        // redraw_line(nr);
        write(1, "L", 1);
        return (1);
    }
    return (5);
}
