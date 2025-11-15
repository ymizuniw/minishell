/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_add_back.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 00:00:00 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/11/15 16:50:15 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// Add element to the end of a generic linked list
void	ft_lst_add_back(void **head, void *new, size_t next_offset)
{
	void	*last;

	if (!head || !new)
		return ;
	if (!*head)
	{
		*head = new;
		return ;
	}
	last = ft_lst_last(*head, next_offset);
	*(void **)((char *)last + next_offset) = new;
}
