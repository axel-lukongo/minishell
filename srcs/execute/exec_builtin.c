/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 12:50:59 by denissereno       #+#    #+#             */
/*   Updated: 2022/09/12 12:50:07 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	my_exit(t_global *g, char **cmd)
{
	if (cmd[1])
	{
		if (ft_strisdigit(cmd[1]) && ft_atoi_u64(cmd[1]) < 9223372036854775807)
		{
			exit_utils(g, cmd);
		}
		else
		{
			error_msg("exit", "numeric argument required");
			ft_malloc_clear(&g->alloc);
			clear_history();
			exit(255);
		}
	}
	else
	{
		ft_malloc_clear(&g->alloc);
		clear_history();
		exit(0);
	}
}

void	my_env(t_global *g, char **cmd)
{
	char	*value;

	if (!cmd[1])
	{
		print_list_env(g->env);
		g->last_return = 0;
	}
	else
	{
		value = get_value_by_name(g->env, cmd[1]);
		if (!value)
		{
			write(2, "env: ", 5);
			write(2, cmd[1], ft_strlen(cmd[1]));
			write(2, ": No such file or directory\n", 28);
			g->last_return = 127;
			return ;
		}
		g->last_return = 0;
	}
}

void	ft_cd(t_global *g, char **cmd)
{
	if (!get_node_by_name(g->env, "PWD"))
		return ;
	if (!ft_strcmp(cmd[0], "cd") && !cmd[1]
		&& !get_value_by_name(g->env, "HOME")
		&& !get_value_by_name(g->export, "HOME"))
		error_msg("cd", "HOME not set");
	if (!cmd[1] && get_node_by_name(g->env, "HOME"))
		my_cd2(g);
	else if (cmd[1] && !dir_change_stack(cmd[1]))
		my_cd(g, cmd);
	else if (dir_change_stack(cmd[1]))
		cd_dash_or_nothing(g, cmd);
}

void	my_pwd(t_global *g, char **cmd)
{
	char	*pwd;

	if (cmd[1] && cmd[1][0] == '-' && cmd[1][1] != 0)
	{
		ft_putstr_fd("minishell: pwd: ", 2);
		ft_putchar_fd(cmd[1][0], 2);
		ft_putchar_fd(cmd[1][1], 2);
		ft_putstr_fd(": invalid option\npwd: usage: pwd [-LP]\n", 2);
		g->last_return = 1;
	}
	else
	{
		pwd = getcwd(NULL, 0);
		printf("%s\n", pwd);
		free(pwd);
		g->last_return = 0;
	}
}

void	execute_builtin(t_global *g, char **cmd)
{
	if (!ft_strcmp(cmd[0], "exit"))
		my_exit(g, cmd);
	else if (!ft_strcmp(cmd[0], "env"))
		my_env(g, cmd);
	else if (!ft_strcmp(cmd[0], "unset"))
		my_unset(g, cmd);
	else if (!ft_strcmp(cmd[0], "cd"))
		ft_cd(g, cmd);
	else if (!ft_strcmp(cmd[0], "export"))
		my_export(g, cmd);
	else if (!ft_strcmp(cmd[0], "dirs"))
	{
		print_ustack(g->dir_stack);
		g->last_return = 0;
	}
	else if (!ft_strcmp(cmd[0], "pwd"))
		my_pwd(g, cmd);
	else if (!ft_strcmp(cmd[0], "echo"))
	{
		ft_echo(cmd);
		g->last_return = 0;
	}
}

// EXPORT
// NAME IDENTIFIER:
/*
	{0, 9}str= -> interdit nombre au debut
	>,<

*/

// FINIR CD : CD /, CD //... A VOIR SUR LE TRUC DE VIET91