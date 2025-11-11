/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_last.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 00:00:00 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/11 10:51:06 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_libft.h"

// Get the last element of a generic linked list
void	*ft_lst_last(void *head, size_t next_offset)
{
	void	*current;
	void	*next_ptr;

	if (!head)
		return (NULL);
	current = head;
	while (1)
	{
		next_ptr = *(void **)((char *)current + next_offset);
		if (!next_ptr)
			break ;
		current = next_ptr;
	}
	return (current);
}
