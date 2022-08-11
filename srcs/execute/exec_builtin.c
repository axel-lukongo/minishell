/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 12:50:59 by denissereno       #+#    #+#             */
/*   Updated: 2022/08/11 01:59:13 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
//test
//TEST
int is_builtin(char *str)
{
    if (!ft_strcmp(str, "echo -n") || !ft_strcmp(str, "export") || !ft_strcmp(str, "cd")
        || !ft_strcmp(str, "unset") || !ft_strcmp(str, "env") || !ft_strcmp(str, "exit")
		|| !ft_strcmp(str, "dirs") || !ft_strcmp(str, "pwd"))
        return (1);
    return (0);
}

int	count_char(char *str, int ch)
{
	int	n;
	int	i;

	i = 0;
	n = 0;
	while (str[i])
	{
		if (str[i] == ch)
			n++;
		i++;
	}
	return (n);
}

char	*del_last_path(t_global *g, char *path)
{
	int		sl;
	int		i;
	char	*new;

	sl = count_char(path, '/');
	i = 0;
	new = ft_malloc(sizeof(char) * ft_strlen(path) + 1, &g->alloc);
	while (path[i] && sl != 0)
	{
		if (path[i] == '/')
			sl--;
		if (sl == 0 && path[i] == '/')
			break ;
		new[i] = path[i];
		i++;
	}
	new[i] = 0;
	return (new);
}

int	dir_change_stack(char *str)
{
	int	i;

	i = 1;
	if (str[0] != '-')
		return (0);
	if (str[0] == '-')
		return (1);
	while (str[i] >= '0' && str[i] <= '9')
		i++;
	if (str[i] != 0)
		return (0);
	return (1);
}

int	is_allowed_var(char *var)
{
	int	i;

	i = 0;
	while (var[i])
	{
		if (!is_shell_char_var_allowed(var[i]))
			return (0);
		i++;
	}
	return (-1);
}
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

void my_exit(t_global *g, char **cmd)
{
	if (cmd[1])
		{ // A FINIR AVEC LA LIMIT 64 
			if (ft_strisdigit(cmd[1]) && ft_atoi_u64(cmd[1]) < 9223372036854775807)
			{
				if (cmd[2] != NULL)
				{
					printf("bash: exit: too many arguments\n");
					g->last_return = 1;
				}
				else
					exit(ft_atoi(cmd[1]) % 256);
			}
			else
			{
				printf("bash: exit: exit: numeric argument required\n");
				exit(255);
			}
		}
		else
			exit(0);
}

void my_env(t_global *g, char **cmd)
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

void my_unset(t_global *g, char **cmd)
{
	if (!cmd[1])
	{
		write(2, "unset: not enough arguments\n", 28);
		return ;
	}
	destroy_env_var(g, cmd[1]);
	g->last_return = 0;
}

void	execute_builtin(t_global *g, char **cmd)
{
	char	*value;
	int		cd;

	if (!ft_strcmp(cmd[0], "exit"))
	{
		my_exit(g, cmd);
		// if (cmd[1])
		// { // A FINIR AVEC LA LIMIT 64 
		// 	if (ft_strisdigit(cmd[1]) && ft_atoi_u64(cmd[1]) < 9223372036854775807)
		// 	{
		// 		if (cmd[2] != NULL)
		// 		{
		// 			printf("bash: exit: too many arguments\n");
		// 			g->last_return = 1;
		// 		}
		// 		else
		// 			exit(ft_atoi(cmd[1]) % 256);
		// 	}
		// 	else
		// 	{
		// 		printf("bash: exit: exit: numeric argument required\n");
		// 		exit(255);
		// 	}
		// }
		// else
		// 	exit(0);
	}
	else if	(!ft_strcmp(cmd[0], "env"))
	{
		my_env(g, cmd);
		// if (!cmd[1])
		// {
		// 	print_list_env(g->env);
		// 	g->last_return = 0;
		// }
		// else
		// {
		// 	value = get_value_by_name(g->env, cmd[1]);
		// 	if (!value)
		// 	{
		// 		write(2, "env: ", 5);
		// 		write(2, cmd[1], ft_strlen(cmd[1]));
		// 		write(2, ": No such file or directory\n", 28);
		// 		g->last_return = 127;
		// 		return ;
		// 	}
		// 	g->last_return = 0;
		// }
	}
	else if	(!ft_strcmp(cmd[0], "unset"))
	{
		my_unset(g, cmd);
	// 	if (!cmd[1])
	// 	{
	// 		write(2, "unset: not enough arguments\n", 28);
	// 		return ;
	// 	}
	// 	destroy_env_var(g, cmd[1]);
	// 	g->last_return = 0;
	}
	else if	(!ft_strcmp(cmd[0], "cd")) // CHANGER PWD PAR GETCWD
	{
		if (!get_node_by_name(g->env, "PWD"))
			return ;
		if (!cmd[1] && get_node_by_name(g->env, "HOME"))
		{
			chdir(get_node_by_name(g->env, "HOME")->value);
			push_ustack(g->dir_stack, get_node_by_name(g->env, "HOME")->value);
			change_value_by_name(g, "PWD", get_node_by_name(g->env, "HOME")->value);
		}
		else if	(cmd[1] && !dir_change_stack(cmd[1]) && cmd[1][0] != '/')
		{
			//printf("%s\n", cmd[1]);
			cd = chdir(ft_strjoin(get_node_by_name(g->env, "PWD")->value, ft_strjoin("/", cmd[1], &g->alloc), &g->alloc)) ;
			if (!cd)
			{
				if (!ft_strcmp(cmd[1], ".."))
				{
					push_ustack(g->dir_stack, del_last_path(g, get_node_by_name(g->env, "PWD")->value));
					change_value_by_name(g, "PWD", ft_strjoin(get_node_by_name(g->env, "PWD")->value, ft_strjoin("/", cmd[1], &g->alloc), &g->alloc));
				}
				else
				{
					push_ustack(g->dir_stack, ft_strjoin(get_node_by_name(g->env, "PWD")->value, ft_strjoin("/", cmd[1], &g->alloc), &g->alloc));
					change_value_by_name(g, "PWD", ft_strjoin(get_node_by_name(g->env, "PWD")->value, ft_strjoin("/", cmd[1], &g->alloc), &g->alloc));
				}
				g->last_return = 0;
			}				
			else
			{
				write(2, "cd: no such file or directory: ", 31);
				write(2, cmd[1], ft_strlen(cmd[1]));
				write(2, "\n", 1);
				g->last_return = 1;
			}
		}
		else if (cmd[1] && !dir_change_stack(cmd[1]) && cmd[1][0] == '/')
		{
			printf("->%s\n", cmd[1]);
			cd = chdir(cmd[1]);
			if (!cd)
			{
				push_ustack(g->dir_stack, ft_strjoin(get_node_by_name(g->env, "HOME")->value, ft_strjoin("/", cmd[1], &g->alloc), &g->alloc));
				change_value_by_name(g, "PWD", cmd[1]);
				g->last_return = 0;
			}				
			else
			{
				write(2, "cd: no such file or directory: ", 31);
				write(2, cmd[1], ft_strlen(cmd[1]));
				write(2, "\n", 1);
				g->last_return = 1;
			}
		}
		else if (dir_change_stack(cmd[1]))
		{
			if (cmd[1][0] == '-' && ft_strlen(cmd[1]) == 1)
			{
				value = get_value_ustack(g->dir_stack, 1);
				if (!value)
					write(2, "cd: no such entry in dir stack\n", 31);
				else
				{
					chdir(value);
					push_ustack(g->dir_stack, value);
					change_value_by_name(g, "PWD", value);
				}
				return ;
			}
			else if (cmd[1][0] == '-' && ft_strlen(cmd[1]) == 2 && cmd[1][1] == '-')
			{
				value = "/Users/axellukongo";
				if (!value)
					write(2, "cd: no such entry in dir stack\n", 31);
				else
				{
					chdir(value);
					push_ustack(g->dir_stack, value);
					change_value_by_name(g, "PWD", value);
				}
				return ;
				// CODER ICI DEPLACER JUSQUAU HOME : CD --
			}
			// printf("---------je rentre ici 1----------\n");
			// cd = ft_atoi(ft_split(cmd[1], '-', g->alloc)[0]);
			// value = get_value_ustack(g->dir_stack, cd);
			// if (!value)
			// 	write(2, "cd: no such entry in dir stack\n", 31);
			// else
			// {
			// 	chdir(value);
			// 	push_ustack(g->dir_stack, value);
			// 	change_value_by_name(g, "PWD", value);
			// }
		}
	}
	else if (!ft_strcmp(cmd[0], "export"))
	{
		char	**env;
		t_env	*node;
		if (!cmd[1])
		{
			//printf("je rentre ici");
			my_export(g->env);
			//print_list_env(g->env);
			g->last_return = 0;
			return ;
		}
		else
		{
			if (!count_char(cmd[1], '=') || cmd[1][0] == '=')
				return ;
			env = ft_split(cmd[1], '=', g->alloc);
			node = ft_malloc(sizeof(*node), &g->alloc);
			if (!env[1])
				*node = (t_env){env[0], ""};
			else
				*node = (t_env){env[0], env[1]};
			if (is_var_env_exist(g->env, env[0]))
				change_value_by_name(g, env[0], node->value);
			else
				ft_lstadd_back(&g->env, ft_lstnew((void *){node}, g->alloc));
		}

	}
	else if (!ft_strcmp(cmd[0], "dirs"))
	{
		print_ustack(g->dir_stack);
		g->last_return = 0;
	}
	else if (!ft_strcmp(cmd[0], "pwd"))
	{
		if (cmd[1] && cmd[1][0] == '-' && cmd[1][1] != 0) // Parsing pas bon (pwd -- p marche);
			printf("bash: pwd: %c%c: invalid option\npwd: usage: pwd [-LP]\n", cmd[1][0], cmd[1][1]);
		else
			printf("%s\n", get_value_by_name(g->env, "PWD"));
	}
}

// EXPORT
// NAME IDENTIFIER:
/*
	{0, 9}str= -> interdit nombre au debut
	>,<

*/


// FINIR CD : CD /, CD //... A VOIR SUR LE TRUC DE VIET91