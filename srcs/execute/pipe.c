/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 17:52:34 by denissereno       #+#    #+#             */
/*   Updated: 2022/09/12 19:04:15 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exec_cmd(t_tree *node, t_global *g, char *cmd)
{
	char	**args;

	if (cmd)
	{
		args = ft_split_quote(cmd, ' ', g->alloc);
		if (is_builtin(args[0]))
			execute_builtin(g, args);
		else
			exec(args, g->env, g);
	}
	else
	{
		args = ft_split_quote(node->value, ' ', g->alloc);
		if (node->value && is_builtin(args[0]))
			execute_builtin(g, args);
		else if (node->value)
			exec(args, g->env, g);
	}
}

void	exec_pipe(t_vector3D cbackp, t_global *g, t_tree *root, char *cmd)
{
	int	fd[2];

	pipe(fd);
	cbackp.z = fork();
	if (cbackp.z == 0)
	{
		signal(SIGQUIT, handle_signale_ctrl_c);
		signal(SIGINT, exit_sig);
		dup2(cbackp.y, 0);
		close(cbackp.y); // a enlever pour corriger
		if (cbackp.x)
			dup2(fd[1], 1);
		close(fd[1]);
		close(fd[0]);
		if (cbackp.x)
			exec_ast(root->left, g, cmd);
		else
			exec_ast(root, g, cmd);
		exit(g->last_return);
	}
	else
		close(fd[1]);
	pipeline(root->right, (t_vector2D){cbackp.x - 1, fd[0]}, g, cmd);
	close(fd[0]);
}

void	pipeline(t_tree *root, t_vector2D cback, t_global *g, char *cmd)
{
	int				fd[2];
	int				pid;

	if (!root)
		return ;
	if (g->hered)
	{
		g->tmpfile++;
		g->hered = 0;
	}
	pid = 0;
	pipe(fd);
	exec_pipe((t_vector3D){cback.x, cback.y, pid}, g, root, cmd);
	pid = wait(NULL);
}

int	is_command(char *cmd, t_global *g)
{
	int		i;
	char	**paths;

	i = 0;
	if (is_builtin(cmd))
		return (1);
	paths = ft_split(get_value_by_name(g->env, "PATH"), ':', &g->alloc);
	while (paths[i])
	{
		paths[i] = ft_strjoin(paths[i], "/", &g->alloc);
		paths[i] = ft_strjoin(paths[i], cmd, &g->alloc);
		if (!access(paths[i], 0))
			return (1);
		i++;
	}
	return (0);
}
