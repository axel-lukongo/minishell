/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 15:24:41 by alukongo          #+#    #+#             */
/*   Updated: 2022/07/09 02:23:52 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../include/minishell.h"

static int unset_first_node(t_list *env, char *name)
{
	int size;
	t_list *current;
	t_list *next_node;
	
	size = ft_lstsize(env);
	current = env;
	next_node = current->next;
	if (size)
	{
		if (ft_strcmp((char *)current->content, name) == 0)
		{
			free(current);
			current = next_node;
			return (1);
		}
	}
	return (0);
}

void ft_unset(t_list *env, char *name)
{
	int size;
	int del_first_node;
	t_list *current;
	t_list *next_node;
	
	del_first_node = unset_first_node(env, name);
	size = ft_lstsize(env);
	current = env;
	while (size && del_first_node != 1)
	{
		next_node = current->next;
		if (next_node && ft_strcmp((char *)next_node->content, name) == 0)
		{
			current->next = next_node->next;
			free(next_node);
			return;
		}
		if (size > 1)
			current = current->next;
		size--;
	}
}
