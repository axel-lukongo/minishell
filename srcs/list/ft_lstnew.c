/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darian <darian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 17:55:39 by dasereno          #+#    #+#             */
/*   Updated: 2022/06/28 13:44:24 by darian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_list	*ft_lstnew(void *content, t_alloc *alloc)
{
	t_list	*li;

	li = ft_malloc(sizeof(*li), &alloc);
	if (!li)
		return (NULL);
	li->content = content;
	li->next = NULL;
	return (li);
}

