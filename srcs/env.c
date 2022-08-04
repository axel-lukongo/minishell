/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 14:30:09 by darian            #+#    #+#             */
/*   Updated: 2022/08/04 02:03:35 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../includes/minishell.h"

t_list	*init_env(char **env, t_alloc *alloc)
{
	int		i;
	char	**strs;
	char	*str;
	t_list	*new_node;
	t_list	*list_env;

	(void) alloc;
	list_env = NULL;
	list_env = 0;
	i = 0;
	while (env[i])
	{
		str = my_strdup(env[i]);
		if(!str)
			return(0);
		strs = my_split(str, '=');
		new_node = ft_lstnew2(strs[0], strs[1]);
		if (!new_node)
			return(0);
		ft_lstadd_back(&list_env, new_node);
		i++;
	}
	return(list_env);
}

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

void	destroy_env_var(t_global *g, char *name)
{
	t_env	*node;
	t_list	*li;
	t_list	*prev;

	li = g->env;
	prev = NULL;
	while (li)
	{
		node = (t_env *)li->content;
		if (!ft_strcmp(node->name, name))
		{
			if (prev == NULL)
				g->env = li;
			else
				prev->next = li->next;
		}
		prev = li;
		li = li->next;
	}
}
