/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 12:47:51 by denissereno       #+#    #+#             */
/*   Updated: 2022/08/30 16:55:32 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	execo(t_exec_con *exec, t_global *g)
{
	(void)exec;
	(void)g;
	// if (exec->nb_cmd == 1)
	// 	ft_pipex(exec->nb_cmd, exec->exec, g);
	// else if (exec->nb_cmd <= 0)
	// 	return ;
	// else
	// {
	// 	ft_pipex(exec->nb_cmd, exec->exec, g);
	// }
}

void	executing(t_exec_con *exec, t_global *g)
{
	if (!ft_strcmp(exec->exec[0]->cmd, "!")
		|| !ft_strcmp(exec->exec[0]->cmd, ":"))
	{
		g->last_return = 0;
		return ;
	}
	execo(exec, g);
}

void	change_return_type(t_exec_con *exec, t_global *g)
{
	if (exec->parent && exec->is_left == 0)
	{
		exec->parent->return_val = g->last_return;
		exec->parent->type = RESULT;
		if (exec->parent->parent && ((exec->parent->parent->left
					&& exec->parent->parent->left->type == RESULT)
				|| !exec->parent->parent ->left)
			&& ((exec->parent->parent->right
					&& exec->parent->parent->right->type == RESULT)
				|| !exec->parent->parent->right))
			exec->parent->parent->type = RESULT;
	}
}

void	execute_branch(char **cmd, t_exec_con *exec, t_global *g)
{
	if (cmd && is_directory(cmd[0]))
	{
		printf("bash: %s: is a directory\n", cmd[0]);
		return ;
	}
	if ((exec->parent && exec->is_left == 0 && g->last_return != 0)
		|| (exec->is_left == 1) || (exec->parent && exec->parent->type == AND)
		|| (!exec->parent))
	{
		if (cmd && is_builtin(cmd[0]))
			execute_builtin(g, cmd);
		else if (cmd)
		{
			if (!(exec->parent && exec->parent->type == AND
					&& g->last_return != 0))
				executing(exec, g);
		}
		else
			return ;
	}
}

void	execute(t_global *g)
{
	t_exec_con	*exec;
	t_tree		*tr;
	char		**cmd;

	printTree(g->ast, NULL, 0);
	tr = get_first_exec_node(g->ast);
	while (count_executable_nodes(g->ast))
	{
		exec = get_exec_container(g, tr);
		if (!exec->exec[0])
			break ;
		cmd = ft_split_quote(exec->exec[0]->cmd, ' ', g->alloc);
		execute_branch(cmd, exec, g);
		change_return_type(exec, g);
		delete_tree(g->ast, exec->node_id, &g->ast);
		tr = get_first_exec_node(g->ast);
	}
}
