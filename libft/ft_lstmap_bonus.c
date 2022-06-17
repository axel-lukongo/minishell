/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 11:52:39 by alukongo          #+#    #+#             */
/*   Updated: 2021/11/29 19:27:04 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"
//good

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*begin;
	t_list	*tmp;

	if (!lst)
		return (0);
	tmp = ft_lstnew((*f)(lst->content));
	if (!tmp)
		return (0);
	begin = tmp;
	while (lst->next)
	{
		lst = lst->next;
		tmp->next = ft_lstnew((*f)(lst->content));
		if (!tmp->next)
		{
			ft_lstclear(&begin, del);
			return (0);
		}
		tmp = tmp->next;
	}
	return (begin);
}
