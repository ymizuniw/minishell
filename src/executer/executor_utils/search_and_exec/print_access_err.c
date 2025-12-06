/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_access_err.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 19:03:05 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/12/06 19:05:11 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	process_access_err(char const *path, int *last_exit_status,
		void (*print_err)(const char *), int status)
{
	print_err(path);
	*last_exit_status = status;
	return (0);
}

void	print_no_such_file_or_dir_err(char const *path)
{
	write(2, "minishell: ", 6);
	write(2, path, ft_strlen(path));
	write(2, ": No such file or directory\n", 28);
}

void	print_permission_denied_err(const char *path)
{
	write(2, "minishell: ", 6);
	write(2, path, ft_strlen(path));
	write(2, ": Permission denied\n", 20);
}

void	print_is_dir_err(const char *path)
{
	perror(path);
}

void	print_stat_err(const char *path)
{
	(void)path;
	perror("stat: ");
}
