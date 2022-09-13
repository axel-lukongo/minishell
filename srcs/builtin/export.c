/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 01:58:09 by alukongo          #+#    #+#             */
/*   Updated: 2022/09/13 11:18:44 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	my_aff_export(t_list *env, t_global *g)
{
	t_list	*cpy_env;
	int		size;

	size = ft_lstsize(env);
	if (!size)
		return ;
	cpy_env = NULL;
	cpy_env = ft_cpy_env(cpy_env, env, size, g);
	ft_list_sort(&cpy_env, cmp);
	print_list_export(cpy_env);
}

void	add_value_export(char **split, t_global *g)
{
	t_env	*node;

	node = ft_malloc(sizeof(*node), &g->alloc2);
	node->name = split[0];
	node->value = NULL;
	if (!is_var_env_exist(g->export, split[0]))
		ft_lstadd_back(&g->export, ft_lstnew((void *){node}, &g->alloc2));
	g_p->last_return = 0;
}

void	export_utils(int i, char **cmd, t_global *g)
{
	char	**split;

	split = ft_split_first(cmd[i], '=', &g->alloc2);
	if (split[0] && split[0][0] == '-')
	{
		ft_putstr_fd("minishell: export: ", 2);
		ft_putchar_fd(split[0][0], 2);
		ft_putchar_fd(split[0][1], 2);
		ft_putstr_fd(": invalid option\n", 2);
		g_p->last_return = 2;
	}
	else if (split[0] && !split[1] && is_valid_identifier(split[0]))
		add_value_export(split, g);
	else if (split[0] && split[1] && is_valid_identifier(split[0]))
		add_value_env(split, g);
	else
		print_err(g, cmd, i);
}

void	my_export(t_global *g, char **cmd)
{
	int		i;

	i = 1;
	if (!cmd[1])
	{
		my_aff_export(g->export, g);
		g_p->last_return = 0;
	}
	while (cmd[i])
		export_utils(i++, cmd, g);
}

void	ft_env(t_list *env)
{
	int	size;

	size = ft_lstsize(env);
	while (size)
	{
		ft_putchar_fd('\n', 1);
		size--;
		if (size > 0)
			env = env->next;
	}
}
