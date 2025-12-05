/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_cntrl.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 03:31:26 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/05 17:48:09 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "new_readline.h"
#include <termcap.h>

int process_cntrl(t_new_readline *nr, char one_char)
{
    (void)nr;
    if (one_char == 0x0C)
    {
        write(1, "L", 1);
        return (1);
    }
    return (5);
}
