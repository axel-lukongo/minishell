/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_set_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 18:03:37 by alukongo          #+#    #+#             */
/*   Updated: 2022/07/08 15:52:03 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../include/minishell.h"

int find_pos_node(t_list *env, char *name)
{
	int	size;
	int i;

	i = 0;
	size = ft_lstsize(env);
	while (size)
	{
		if (ft_strcmp(env->content, name) == 0)
			return (i);
		size--;
		if (size > 0)
			env = env->next;
		i++;
	}
	return (ERROR);
}

void	set_env(t_list *env, char *name, char *result)
{
	int	pos;
	t_list *lst;
	
	lst = env;
	pos = find_pos_node(env, name);
	while (pos)
	{
		lst = lst->next;
		pos--;
	}
	if (pos != ERROR)
		lst->result = result;
}