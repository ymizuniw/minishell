/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   restore_stdio.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 17:37:58 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/02 17:16:47 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	restore_stdio(int stdin_keep, int stdout_keep)
{
	dup2(stdin_keep, STDIN_FILENO);
	dup2(stdout_keep, STDOUT_FILENO);
	xclose(stdin_keep);
	xclose(stdout_keep);
}
