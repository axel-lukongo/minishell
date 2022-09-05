/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/11 19:41:14 by alukongo          #+#    #+#             */
/*   Updated: 2022/09/05 19:23:20 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	cd_error_msg(t_global *g, char **cmd)
{
	if (!is_directory(cmd[1]))
	{
		printf("cd: %s: Not a directory\n", cmd[1]);
		g->last_return = 1;
	}
	else
	{
		write(2, "cd: no such file or directory: ", 31);
		write(2, cmd[1], ft_strlen(cmd[1]));
		write(2, "\n", 1);
		g->last_return = 1;
	}
}

void	my_cd2(t_global *g)
{
	change_value_by_name(g, "OLDPWD", get_node_by_name(g->env, "PWD")->value);
	chdir(get_node_by_name(g->env, "HOME")->value);
	push_ustack(g->dir_stack, get_node_by_name(g->env, "HOME")->value);
	change_value_by_name(g, "PWD", getcwd((char *)NULL, 0));
}

void	cd_dash_or_nothing(t_global *g, char **cmd)
{
	char	*value;

	if	(cmd[1][0] == '-' && ft_strlen(cmd[1]) == 1)
	{
		value = get_value_ustack(g->dir_stack, 1);
		if	(!value)
			write(2, "cd: no such entry in dir stack\n", 31);
		else
		{
			chdir(value);
			push_ustack(g->dir_stack, value);
			change_value_by_name(g, "PWD", getcwd((char *)NULL, 0));
		}
		return ;
	}
	else if	(cmd[1][0] == '-' && ft_strlen(cmd[1]) == 2 && cmd[1][1] == '-')
		my_cd2(g);
}

void	my_cd(t_global *g, char **cmd)
{
	int	cd;

	if	(cmd[1] && !dir_change_stack(cmd[1]))
	{
		change_value_by_name(g, "OLDPWD", get_node_by_name(g->env, "PWD")->value);
		if	(cmd[1][0] != '/')
			cd = chdir(ft_strjoin(get_node_by_name(g->env, "PWD")->value, ft_strjoin("/", cmd[1], &g->alloc), &g->alloc)) ;
		else
			cd = chdir(cmd[1]);
		if	(!cd)
		{
			if (!ft_strcmp(cmd[1], ".."))
			{
				push_ustack(g->dir_stack, del_last_path(g, get_node_by_name(g->env, "PWD")->value));
				change_value_by_name(g, "PWD", getcwd((char *)NULL, 0));
			}
			else
			{
				push_ustack(g->dir_stack, ft_strjoin(get_node_by_name(g->env, "PWD")->value, ft_strjoin("/", cmd[1], &g->alloc), &g->alloc));
				change_value_by_name(g, "PWD", getcwd((char *)NULL, 0));
			}
			g->last_return = 0;
		}
		else
			cd_error_msg(g, cmd);
	}
}
