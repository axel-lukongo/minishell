/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/10 16:46:39 by denissereno       #+#    #+#             */
/*   Updated: 2022/09/11 15:11:40 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_list	*init_env(char **env, t_alloc **alloc, int i)
{
	char	**strs;
	char	*str;
	t_list	*new_node;
	t_list	*list_env;
	t_env	*node;

	list_env = 0;
	while (env[i])
	{
		str = ft_strdup(env[i], alloc);
		if (!str)
			return (0);
		strs = ft_split(str, '=', alloc);
		node = ft_malloc(sizeof(*node), alloc);
		*node = (t_env){strs[0], strs[1]};
		new_node = ft_lstnew((void *){node}, alloc);
		if (!new_node)
			return (0);
		ft_lstadd_back(&list_env, new_node);
		i++;
	}
	return (list_env);
}

void	error_msg(char	*arg, char *message)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(message, 2);
	ft_putstr_fd("\n", 2);
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

void	change_shlvl(t_global *g)
{
	char	*var;

	var = get_value_by_name(g->env, "SHLVL");
	if (!var)
		return ;
	change_value_by_name(g->env, "SHLVL", ft_itoa(ft_atoi(var) + 1, g->alloc));
	change_value_by_name(g->export, "SHLVL", ft_itoa(ft_atoi(var) + 1,
			g->alloc));
	return ;
}

char	**env_to_tab(t_list *env, t_alloc **alloc)
{
	char	**envp;
	t_env	*node;
	int		i;
	char	*tmp;

	envp = ft_malloc(sizeof(char *) * (ft_lstsize(env) + 1), alloc);
	i = 0;
	while (env)
	{
		node = (t_env *)env->content;
		tmp = ft_strjoin(node->name, "=", alloc);
		tmp = ft_strjoin(tmp, node->value, alloc);
		envp[i] = ft_strdup(tmp, alloc);
		i++;
		env = env->next;
		tmp = NULL;
	}
	envp[i] = NULL;
	return (envp);
}
