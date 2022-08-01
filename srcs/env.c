/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 14:30:09 by darian            #+#    #+#             */
/*   Updated: 2022/07/19 15:01:19 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../includes/minishell.h"

t_list *init_env(char **env, t_alloc *alloc)
{
	int i;
	char **strs;
	char *str;
	t_list *new_node;
	t_list *list_env;
	t_env	*node;
	
	list_env = 0;
	i = 0;
	while (env[i])
	{
		str = ft_strdup(env[i], alloc);
		if(!str)
			return(0);
		strs = ft_split(str, '=', alloc);
		node = ft_malloc(sizeof(*node), &alloc);
		*node = (t_env){strs[0], strs[1]};
		new_node = ft_lstnew((void *){node}, alloc);
		if (!new_node)
			return (0);
		ft_lstadd_back(&list_env, new_node);
		i++;
	}
	// FOR TESTING EXPANDING
	node = ft_malloc(sizeof(*node), &alloc);
	*node = (t_env){"A", "ech"};
	new_node = ft_lstnew((void *){node}, alloc);
	ft_lstadd_back(&list_env, new_node);
	node = ft_malloc(sizeof(*node), &alloc);
	*node = (t_env){"B", "o"};
	new_node = ft_lstnew((void *){node}, alloc);
	ft_lstadd_back(&list_env, new_node);
	// END TEST
	// print_list_env(list_env);
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
