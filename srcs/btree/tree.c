/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: darian <darian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 16:54:32 by darian            #+#    #+#             */
/*   Updated: 2022/06/29 15:09:21 by darian           ###   ########.fr       */
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
	// if (tr->value)
	// 	free(tr->value);
	printf("%s\n", tr->value);
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


void	print_tree_prefix(t_tree *tr, int *space)
{
	if (tr == NULL)
	{
		printf("tree empty\n");
		return ;
	}
	if (tr->parent != NULL)
	{
		for (int i = 0; i < *space * 4; i++)
			printf(" ");
		printf("(%d, %s) -> (%d, %s)\n", tr->parent->type, tr->parent->value, tr->type, tr->value);
	}
	else
	{
		for (int i = 0; i < *space * 4; i++)
			printf(" ");
		printf("(%d, %s)\n", tr->type, tr->value);
	}
	if (tr->right)
	{
		*space += 1;
		print_tree_prefix(tr->right, space);
	}
	if (tr->left)
	{
		*space += 1;
		print_tree_prefix(tr->left, space);
	}
	*space -= 1;
}

void	print_tree(t_tree *tr)
{
	int	space;

	space = 0;
	print_tree_prefix(tr, &space);
}

int	count_tree_nodes(t_tree *tr)
{
	if (!tr)
		return (0);
	return (count_tree_nodes(tr->left) + count_tree_nodes(tr->right) + 1);
}
