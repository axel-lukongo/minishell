/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 11:48:10 by alukongo          #+#    #+#             */
/*   Updated: 2021/11/26 16:25:11 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"
//good
void	ft_lstadd_back(t_list **alst, t_list *new)
{
	t_list	*li;

	li = *alst;
	if (!li)
		*alst = new;
	else
	{
		while (li->next != NULL)
		{
			li = li->next;
		}
		li->next = new;
	}
}
