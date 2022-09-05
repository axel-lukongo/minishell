/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 14:30:09 by darian            #+#    #+#             */
/*   Updated: 2022/08/31 13:33:45 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../includes/minishell.h"

t_env	*get_node_by_name(t_list *env, char *name)
{
	t_env	*node;

	while (env)
	{
		node = (t_env *)env->content;
		if (ft_strncmp(name, node->name, 32767) == 0)
			return (node);
		env = env->next;
	}
	return (NULL);
}

char	*get_value_by_name(t_list *env, char *name)
{
	t_env	*node;

	while (env)
	{
		node = (t_env *)env->content;
		if (ft_strncmp(name, node->name, 32767) == 0)
			return (node->value);
		env = env->next;
	}
	return (NULL);
}

int	is_var_env_exist(t_list	*env, char *name)
{
	t_env	*node;

	while (env)
	{
		node = (t_env *)env->content;
		if (ft_strncmp(name, node->name, 32767) == 0)
			return (1);
		env = env->next;
	}
	return (0);
}

void	change_value_by_name(t_global *g, char *name, char *value)
{
	t_env	*node;
	t_list	*env;

	env = g->env;
	while (env)
	{
		node = (t_env *)env->content;
		if (ft_strncmp(name, node->name, 32767) == 0)
			node->value = value;
		env = env->next;
	}
}

void	change_value_or_add_it(t_global *g, t_list **env, char *name
	, char *value)
{
	t_env	*node;
	t_list	*buf;
	t_env	*new;

	buf = *env;
	while (buf)
	{
		node = (t_env *)buf->content;
		if (ft_strncmp(name, node->name, 32767) == 0)
		{
			node->value = value;
			return ;
		}
		buf = buf->next;
	}
	new = ft_malloc(sizeof(*new), &g->alloc);
	new->name = name;
	new->value = value;
	ft_lstadd_back(env, ft_lstnew((void *)new, &g->alloc));
}

void	destroy_env_var(t_list	**lst, char *name)
{
	t_env	*node;
	t_list	*li;
	t_list	*prev;

	li = *lst;
	prev = NULL;
	while (li)
	{
		node = (t_env *)li->content;
		if (!ft_strcmp(node->name, name))
		{
			if (prev == NULL)
				*lst = li;
			else
				prev->next = li->next;
		}
		prev = li;
		li = li->next;
	}
}
