/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 18:42:07 by alukongo          #+#    #+#             */
/*   Updated: 2022/09/11 18:54:34 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_list	*ft_lstnew2(void *content, void *result, t_alloc **alloc)
{
	t_list	*ptr;

	ptr = ft_malloc(sizeof(t_list), alloc);
	if (!ptr)
		return (0);
	ptr->next = 0;
	ptr->content = content;
	ptr->result = result;
	return (ptr);
}

int	cmp(void *content, void *content_ref)
{
	size_t i;
	char *str;
	char *str2;

	str2 = ((t_env *)content_ref)->name;
	str = ((t_env *)content)->name;
	i = 0;
	while (i < ft_strlen(str) && i < ft_strlen(str2))
	{
		if (str[i] > str2[i])
			return (1);
		else if (str[i] < str2[i])
			return (-1);
		i++;
	}
	return (-1);
}

void	ft_list_sort(t_list **begin_list, int (*cmp)())
{
	t_list *current;
	void *next_list_content;
	current = *begin_list;
	while (current->next)
	{
		if ((*cmp)(current->content, current->next->content) > 0)
		{
			next_list_content = current->content;
			current->content = current->next->content;
			current->next->content = next_list_content;
			current = *begin_list; 
		}
		else
			current = current->next;
	}
}

t_list	*ft_cpy_env(t_list *dest, t_list *src, int src_size, t_global *g)
{
	t_list *list1;
	t_list *list2;
	
	list1 = dest;
	list2 = src;
	while (src_size)
	{
		list1 = ft_lstnew2(list2->content, list2->result, &g->alloc);
		if (!list1)
			return(0);
		ft_lstadd_back(&dest, list1);
		list2 = list2->next;
		src_size--;
	}
	return (dest);
}

void	add_value_env(char **split, t_global *g)
{
	t_env	*node;

	node = ft_malloc(sizeof(*node), &g->alloc);
	node->name = split[0];
	node->value = del_start_end_char(split[1], ' ', &g->alloc);
	change_value_or_add_it(g, &g->export, split[0], node->value);
	change_value_or_add_it(g, &g->env, split[0], node->value);
	g->last_return = 0;
}
