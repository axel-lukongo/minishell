/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_op.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 17:51:40 by denissereno       #+#    #+#             */
/*   Updated: 2022/09/11 17:52:13 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	choose_exec_type(t_tree *root, t_global *g, char *cmd)
{
	int	pid;

	if (root && ((root->type >= 0 && root->type <= 3) || (root->type == 14)
			|| (root->type >= 20 && root->type <= 24)))
	{
		if (root->value && is_builtin(ft_split_quote(root->value, ' ',
					g->alloc)[0]))
			execute_builtin(g, ft_split_quote(root->value, ' ', g->alloc));
		else if (root->value)
		{
			pid = fork();
			signal(SIGINT, SIG_IGN);
			if (pid == 0)
			{
				signal(SIGINT, sig);
				exec(ft_split_quote(root->value, ' ', g->alloc), g->env, g);
			}
			else
				ft_waitpid(g, pid);
		}
	}
	else
		exec_ast(root, g, cmd);
}

void	exec_and(t_tree *node, t_global *g, char *cmd)
{
	choose_exec_type(node->left, g, cmd);
	if (g->hered)
	{
		g->tmpfile++;
		g->hered = 0;
	}
	if (g->last_return == 0)
		choose_exec_type(node->right, g, cmd);
	if (g->hered)
	{
		g->tmpfile++;
		g->hered = 0;
	}
}

int	is_type_in_btree_limited(t_tree *root, int type, int count)
{
	if (!root)
		return (count);
	if (root->left && root->left->type != OR && root->left->type != AND)
	{
		if (root->type == type)
			return (is_type_in_btree_limited(root->left, type, count + 1));
		else
			return (is_type_in_btree_limited(root->left, type, count));
	}
	if (root->right && root->right->type != OR && root->right->type != AND)
	{
		if (root->type == type)
			return (is_type_in_btree_limited(root->right, type, count + 1));
		else
			return (is_type_in_btree_limited(root->right, type, count));
	}
	return (count);
}

void	exec_or(t_tree *node, t_global *g, char *cmd)
{
	if (g->hered)
	{
		g->tmpfile++;
		g->hered = 0;
	}
	choose_exec_type(node->left, g, cmd);
	if (g->hered)
	{
		g->tmpfile++;
		g->hered = 0;
	}
	if (g->last_return != 0)
		choose_exec_type(node->right, g, cmd);
	else
	{
		if (is_type_in_btree_limited(node->right, DLESS, 0))
			g->tmpfile++;
	}
}
