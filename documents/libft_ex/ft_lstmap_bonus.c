/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 18:58:25 by ymizuniw          #+#    #+#             */
/*   Updated: 2025/05/06 21:31:21 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*mapped_lst;
	t_list	*mapped_node;
	t_list	*tail;

	if (!lst || !f || !del)
		return (NULL);
	mapped_lst = NULL;
	tail = NULL;
	while (lst)
	{
		mapped_node = ft_lstnew(f(lst->content));
		if (!mapped_node)
		{
			ft_lstclear(&mapped_lst, del);
			return (NULL);
		}
		if (!mapped_lst)
			mapped_lst = mapped_node;
		else
			tail->next = mapped_node;
		tail = mapped_node;
		lst = lst->next;
	}
	return (mapped_lst);
}

// 	{
	// 	mapped_node = lstnew(f(lst->content));
	// 	if (!mapped_node)
	// 	{
	// 		ft_lstclear(&mapped_lst, del);
	// 		return (NULL);
	// 	}
	// 	ft_lstadd_back(&mapped_lst, mapped_node);
	// 	lst = lst->next;
	// }
