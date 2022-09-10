/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 12:50:59 by denissereno       #+#    #+#             */
/*   Updated: 2022/09/08 15:46:47 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
//test
//TEST
// int	is_builtin(char *str)
// {
// 	if	(!ft_strcmp(str, "echo -n") || !ft_strcmp(str, "export") || !ft_strcmp(str, "cd")
// 		|| !ft_strcmp(str, "unset") || !ft_strcmp(str, "env") || !ft_strcmp(str, "exit")
// 		|| !ft_strcmp(str, "dirs") || !ft_strcmp(str, "pwd"))
// 		return (1);
// 	return (0);
// }

// int	count_char(char *str, int ch)
// {
// 	int	n;
// 	int	i;

// 	i = 0;
// 	n = 0;
// 	while	(str[i])
// 	{
// 		if	(str[i] == ch)
// 			n++;
// 		i++;
// 	}
// 	return (n);
// }

// char	*del_last_path(t_global *g, char *path)
// {
// 	int		sl;
// 	int		i;
// 	char	*new;

// 	sl = count_char(path, '/');
// 	i = 0;
// 	new = ft_malloc(sizeof(char) * ft_strlen(path) + 1, &g->alloc);
// 	while	(path[i] && sl != 0)
// 	{
// 		if	(path[i] == '/')
// 			sl--;
// 		if	(sl == 0 && path[i] == '/')
// 			break ;
// 		new[i] = path[i];
// 		i++;
// 	}
// 	new[i] = 0;
// 	return (new);
// }

// int	dir_change_stack(char *str)
// {
// 	int	i;

// 	i = 1;
// 	if (str[0] != '-')
// 		return (0);
// 	if (str[0] == '-')
// 		return (1);
// 	while (str[i] >= '0' && str[i] <= '9')
// 		i++;
// 	if (str[i] != 0)
// 		return (0);
// 	return (1);
// }

// int	is_allowed_var(char *var)
// {
// 	int	i;

// 	i = 0;
// 	while	(var[i])
// 	{
// 		if	(!is_shell_char_var_allowed(var[i]))
// 			return (0);
// 		i++;
// 	}
// 	return	(-1);
// }
/*
static void error_msg(char *path)
{
	char *err;
	char *msg;
	
	err =  strerror(errno);
	msg = my_strjoin(": ", err);
	ft_putstr_fd("mishell: cd: ", 1);
	ft_putstr_fd(path, 1);
	ft_putstr_fd(err, 1);
}
*/

void	my_exit(t_global *g, char **cmd)
{
	int	tmp;

	if	(cmd[1])
		{ // A FINIR AVEC LA LIMIT 64 
			if	(ft_strisdigit(cmd[1]) && ft_atoi_u64(cmd[1]) < 9223372036854775807)
			{
				if	(cmd[2] != NULL)
				{
					printf("bash: exit: too many arguments\n");
					g->last_return = 1;
				}
				else
				{
					clear_history();
					tmp = ft_atoi(cmd[1]) % 256;
					ft_malloc_clear(&g->alloc);
					exit(tmp);
				}
			}
			else
			{
				printf("bash: exit: exit: numeric argument required\n");
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

	if	(!cmd[1])
		{
			print_list_env(g->env);
			g->last_return = 0;
		}
		else
		{
			value = get_value_by_name(g->env, cmd[1]);
			if	(!value)
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

void ft_cd(t_global *g, char **cmd)
{
	if (!get_node_by_name(g->env, "PWD"))
		return ;
	if (!ft_strcmp(cmd[0], "cd") && !cmd[1]
		&& !get_value_by_name(g->env, "HOME")
		&& !get_value_by_name(g->export, "HOME"))
		error_msg("cd", "HOME not set");
	if (!cmd[1] && get_node_by_name(g->env, "HOME"))
		my_cd2(g);
	else if	(cmd[1] && !dir_change_stack(cmd[1]))
		my_cd(g, cmd);
	else if (dir_change_stack(cmd[1]))
		cd_dash_or_nothing(g, cmd);
}

void	my_pwd(t_global *g, char **cmd)
{
	char	*pwd;

	if	(cmd[1] && cmd[1][0] == '-' && cmd[1][1] != 0) // Parsing pas bon (pwd -- p marche);
	{
		printf("bash: pwd: %c%c: invalid option\npwd: usage: pwd [-LP]\n", cmd[1][0], cmd[1][1]);
		g->last_return = 1;
	}
	else
	{
		pwd =  getcwd(NULL, 0);
		printf("%s\n", pwd);
		free(pwd);
		g->last_return = 0;
	}
}

void	execute_builtin(t_global *g, char **cmd)
{
	if	(!ft_strcmp(cmd[0], "exit"))
		my_exit(g, cmd);
	else if	(!ft_strcmp(cmd[0], "env"))
		my_env(g, cmd);
	else if	(!ft_strcmp(cmd[0], "unset"))
		my_unset(g, cmd);
	else if	(!ft_strcmp(cmd[0], "cd")) // CHANGER PWD PAR GETCWD
		ft_cd(g, cmd);
	else if	(!ft_strcmp(cmd[0], "export"))
		my_export(g, cmd);
	else if	(!ft_strcmp(cmd[0], "dirs"))
	{
		print_ustack(g->dir_stack);
		g->last_return = 0;
	}
	else if	(!ft_strcmp(cmd[0], "pwd"))
	{

	}
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