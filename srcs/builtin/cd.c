/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/11 19:41:14 by alukongo          #+#    #+#             */
/*   Updated: 2022/09/11 20:11:34 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	cd_error_msg(t_global *g, char **cmd)
{
	if (!is_directory(cmd[1]))
	{
		error_msg_double("cd", cmd[1], "Not a directory");
		g->last_return = 1;
	}
	else
	{
		error_msg_double("cd", cmd[1], "No such file or directory");
		g->last_return = 1;
	}
}

void	my_cd2(t_global *g)
{
	char	*pwd;

	change_value_by_name(g->env, "OLDPWD", get_node_by_name(g->env, "PWD")->value);
	chdir(get_node_by_name(g->env, "HOME")->value);
	pwd = getcwd((char *)NULL, 0);
	push_ustack(g->dir_stack, get_node_by_name(g->env, "HOME")->value);
	change_value_by_name(g->env, "PWD", pwd);
	free(pwd);
}

void	cd_dash_or_nothing(t_global *g, char **cmd)
{
	char	*value;
	char	*pwd;

	if	(cmd[1][0] == '-' && ft_strlen(cmd[1]) == 1)
	{
		value = get_value_ustack(g->dir_stack, 1);
		if	(!value)
			write(2, "cd: no such entry in dir stack\n", 31);
		else
		{
			chdir(value);
			push_ustack(g->dir_stack, value);
			pwd = getcwd((char *)NULL, 0);
			change_value_by_name(g->env, "PWD", pwd);
			free(pwd);
		}
		return ;
	}
	else if	(cmd[1][0] == '-' && ft_strlen(cmd[1]) == 2 && cmd[1][1] == '-')
		my_cd2(g);
}

void cd_utils(t_global *g, char **cmd)
{
	char	*pwd;
	
	if (!ft_strcmp(cmd[1], ".."))
	{
		push_ustack(g->dir_stack, del_last_path(g, get_node_by_name(g->env, "PWD")->value));
		pwd = getcwd((char *)NULL, 0);
		change_value_by_name(g->env, "PWD", pwd);
		free(pwd);
	}
	else
	{
		pwd = getcwd((char *)NULL, 0);
		push_ustack(g->dir_stack, ft_strjoin(get_node_by_name(g->env, "PWD")->value, ft_strjoin("/", cmd[1], &g->alloc), &g->alloc));
		change_value_by_name(g->env, "PWD", pwd);
		free(pwd);
	}
}

void	my_cd(t_global *g, char **cmd)
{
	int		cd;

	if	(cmd[1] && !dir_change_stack(cmd[1]))
	{
		change_value_by_name(g->env, "OLDPWD", get_node_by_name(g->env, "PWD")->value);
		if	(cmd[1][0] != '/')
			cd = chdir(ft_strjoin(get_node_by_name(g->env, "PWD")->value, ft_strjoin("/", cmd[1], &g->alloc), &g->alloc)) ;
		else
			cd = chdir(cmd[1]);
		if	(!cd)
		{
			cd_utils(g, cmd);
			g->last_return = 0;
		}
		else
			cd_error_msg(g, cmd);
	}
}
