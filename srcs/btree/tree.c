/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 16:54:32 by darian            #+#    #+#             */
/*   Updated: 2022/09/11 15:53:30 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_tree	*new_tree(t_token tok, t_global *g)
{
	t_tree	*tr;

	tr = ft_malloc(sizeof(*tr), &g->alloc);
	if (tr == NULL)
		return (NULL);
	tr->type = tok.type;
	tr->value = tok.str;
	tr->id = g->node_id++;
	tr->left = NULL;
	tr->right = NULL;
	tr->parent = NULL;
	return (tr);
}

void	clean_tree_a(t_tree *tr)
{
	if (tr == NULL)
		return ;
	clean_tree_a(tr->left);
	clean_tree_a(tr->right);
	tr->left = NULL;
	tr->right = NULL;
	tr->parent = NULL;
	tr->value = NULL;
	free(tr);
	tr = NULL;
}

void	clean_tree(t_tree **tr)
{
	clean_tree_a(*tr);
}

t_tree	*join_tree(t_tree *left, t_tree *right, t_token tok, t_global *g)
{
	t_tree	*tr;

	tr = new_tree(tok, g);
	if (!tr)
		return (NULL);
	tr->left = left;
	tr->right = right;
	if (left != NULL)
		left->parent = tr;
	if (right != NULL)
		right->parent = tr;
	return (tr);
}

int	count_tree_nodes(t_tree *tr)
{
	if (!tr)
		return (0);
	return (count_tree_nodes(tr->left) + count_tree_nodes(tr->right) + 1);
}
