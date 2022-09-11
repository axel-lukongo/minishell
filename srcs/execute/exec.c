/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/30 14:00:24 by denissereno       #+#    #+#             */
/*   Updated: 2022/09/11 17:56:56 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_cmd_tree_loop(t_tree *r, t_alloc **al, int t1, int t2)
{
	char	*res;

	res = NULL;
	while (r->parent && (r->parent->type == t1 || r->parent->type == t2))
	{
		if (r->left && ((r->left->type >= 0 && r->left->type <= 3)
				|| (r->left->type >= 20 && r->left->type <= 24)))
		{
			res = ft_strjoin(res, " ", al);
			res = ft_strjoin(res, r->left->value, al);
		}
		if (r->right && ft_split_first(r->right->value, ' ', al)[1])
		{
			res = ft_strjoin(res, " ", al);
			res = ft_strjoin(res, ft_split_first(r->right->value, ' ',
						al)[1], al);
		}
		r = r->parent;
	}
	res = ft_strjoin(res, " ", al);
	res = ft_strjoin(res, ft_split_first(r->right->value, ' ', al)[1], al);
	return (res);
}

char	*get_cmd_tree(t_tree *r, int type1, int type2, t_alloc **alloc)
{
	char	*res;

	res = NULL;
	if ((!r->parent || (r->parent->type != type1 && r->parent->type
				!= type2)) && (r->type == type1 || r->type == type2))
	{
		if (r->left && ((r->left->type >= 0 && r->left->type <= 3)
				|| (r->left->type >= 20 && r->left->type <= 24)))
		{
			res = ft_strjoin(res, " ", alloc);
			res = ft_strjoin(res, r->left->value, alloc);
		}
		if (r->right && ft_split_first(r->right->value, ' ', alloc)[1])
		{
			res = ft_strjoin(res, " ", alloc);
			res = ft_strjoin(res, ft_split_first(r->right->value, ' ',
						alloc)[1], alloc);
		}
		return (res);
	}
	else if (r->parent && (r->parent->type == type1
			|| r->parent->type == type2))
		return (get_cmd_tree_loop(r, alloc, type1, type2));
	else
		return (NULL);
}

void	exit_sig(int sig)
{
	(void)sig;
	exit(errno % 256);
}

void	exec_ast(t_tree *ast, t_global *g, char *cmd)
{
	t_tree	*node;

	node = ast;
	if (ast == 0)
		return ;
	if (node->type == AND)
		exec_and(node, g, cmd);
	else if (node->type == OR)
		exec_or(node, g, cmd);
	else if ((node->type >= 0 && node->type <= 3)
		|| (node->type >= 20 && node->type <= 24) || node->type == WILDCARD)
		exec_cmd(node, g, cmd);
	else if (node->type >= 9 && node->type <= 12)
		redirections(node, g, cmd);
	else if (node->type == PIPE)
		pipeline(node, (t_vector2D){1, -1}, g, cmd);
	else
		return ;
}
