/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 11:51:19 by alukongo          #+#    #+#             */
/*   Updated: 2021/11/26 12:14:21 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"
//good
void	ft_lstdelone(t_list *lst, void (*del)(void*))
{
	if (lst)
	{
		(del)(lst->content);
		free(lst);
		lst = 0;
	}
}
