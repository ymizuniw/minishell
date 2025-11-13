/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:19:59 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/05/06 21:28:49 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*new_ptr;

	new_ptr = malloc(sizeof(t_list));
	if (!new_ptr)
		return (NULL);
	new_ptr->content = content;
	new_ptr->next = NULL;
	return (new_ptr);
}
