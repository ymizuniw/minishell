/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 18:35:02 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/02 17:16:03 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Minimal get_pwd implementation returning current working directory
// #include <minishell.h>
#include <minishell.h>

char	*get_pwd(void)
{
	return (getcwd(NULL, 0));
}
