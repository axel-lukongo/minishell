/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_index.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 15:38:22 by denissereno       #+#    #+#             */
/*   Updated: 2022/09/12 13:12:35 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_index(t_list **alst, t_list *new, int index)
{
	t_list	*tmp;
	t_list	*prev;
	int		i;

	tmp = *alst;
	i = 0;
	if (index == 0)
	{
		ft_lstadd_front(alst, new);
		return ;
	}
	else if (index == ft_lstsize(*alst) - 1)
	{
		ft_lstadd_back(alst, new);
		return ;
	}
	if (tmp == NULL)
	{
		*alst = new;
		return ;
	}
	while (i != index - 1)
	{
		tmp = tmp->next;
		i++;
	}
	prev = tmp->next;
	tmp->next = new;
	new->next = prev;
	tmp->next = new;
}
