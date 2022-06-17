/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 11:49:51 by alukongo          #+#    #+#             */
/*   Updated: 2021/11/26 12:13:24 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"
//good
void	ft_lstadd_front(t_list **alst, t_list *new)
{
	if (alst)
	{
		new->next = *alst;
	}
	else
		new->next = NULL;
	*alst = new;
}
