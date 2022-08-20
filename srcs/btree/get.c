/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 19:10:01 by darian            #+#    #+#             */
/*   Updated: 2022/08/19 15:23:05 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_tree	*get_first_exec_node(t_tree *tr)
{
	if (!tr)
		return (NULL);
	while (1)
	{
		if (tr && (tr->type == AND || tr->type == OR) && tr->left
			&& tr->left->type != RESULT)
			tr = tr->left;
		else if (tr && (tr->type == AND || tr->type == OR)
			&& tr->right && tr->right->type != RESULT)
			tr = tr->right;
		else
			return (tr);
	}
	return (NULL);
}

void	delete_tree(t_tree *node, int id, t_tree **root)
{
	if (node->id == id)
	{
		if (node->id == (*root)->id)
		{
			(*root)->left = NULL;
			(*root)->value = NULL;
			(*root)->right = NULL;
			(*root) = NULL;
		}
		if (node->parent)
		{
			if (node->parent->left)
			{
				if (node->parent->left->id == node->id)
					node->parent->left = NULL;
			}
			if (node->parent->right)
			{
				if (node->parent->right->id == node->id)
					node->parent->right = NULL;
			}
		}
		node->left = NULL;
		node->value = NULL;
		node->right = NULL;
		node = NULL;
		return ;
	}
	if (node->left)
		delete_tree(node->left, id, root);
	if (node->right)
		delete_tree(node->right, id, root);
}

int	count_executable_nodes(t_tree	*tr)
{
	int	count;

	if (!tr)
		return (0);
	if (tr->type == CMD || tr->type == WILDCARD || tr->type == ENV
		|| tr->type == FILE || tr->type == WILDENV || tr->type == BACKSLASH
		|| tr->type == BACKENV || tr->type == WILDBACK
		|| tr->type == WILDENVBACK)
		count = 1;
	else
		count = 0;
	if (tr->left)
		count += count_executable_nodes(tr->left);
	if (tr->right)
		count += count_executable_nodes(tr->right);
	return (count);
}

t_tree	*get_last_node(t_tree	*tr)
{
	if (!tr)
		return (NULL);
	if (tr->left)
		return (get_last_node(tr->left));
	if (tr->right)
		return (get_last_node(tr->right));
	return (tr);
}

t_exec	**create_exec_tab(t_global *g, t_tree *tr)
{
	t_exec	**exec;
	t_tree	*sub;
	int		i;

	exec = ft_malloc(sizeof(t_exec *) * (count_executable_nodes(tr) + 1),
			&g->alloc);
	i = 0;
	while (tr)
	{
		sub = get_last_node(tr);
		if (!sub || !sub->value)
			break ;
		else if (sub->type != CMD && sub->type != ENV && sub->type != WILDCARD
			&& sub->type != WILDENV && sub->type != BACKSLASH && sub->type
			!= BACKENV && sub->type != WILDENVBACK && sub->type != WILDBACK
			&& sub->type != FILE && count_tree_nodes(tr) == 1)
		{
			delete_tree(tr, sub->id, &tr);
			break ;
		}
		else if (sub->type != CMD && sub->type != ENV && sub->type != WILDCARD
			&& sub->type != WILDENV && sub->type != BACKSLASH && sub->type
			!= BACKENV && sub->type != WILDENVBACK && sub->type != WILDBACK
			&& sub->type != FILE && count_tree_nodes(tr) != 1)
			delete_tree(tr, sub->id, &tr);
		else
		{
			exec[i] = ft_malloc(sizeof(*exec[i]), &g->alloc);
			exec[i]->cmd = ft_strdup(sub->value, g->alloc);
			if (sub->parent)
				exec[i]->redirect_last = sub->parent->type;
			else
				exec[i]->redirect_last = -1;
			delete_tree(tr, sub->id, &tr);
			i++;
		}
	}
	if (i != 0)
		exec[0]->redirect_last = -1;
	exec[i] = NULL;
	return (exec);
}

t_exec_con	*get_exec_container(t_global *g, t_tree *tr)
{
	t_exec_con	*cont;

	if (!g->ast)
		return (NULL);
	cont = ft_malloc(sizeof(*cont), &g->alloc);
	cont->return_val = 0;
	cont->parent = tr->parent;
	if (tr->parent)
	{
		if (tr->parent->left)
		{
			if (tr->parent->left->id == tr->id)
				cont->is_left = 1;
			else
				cont->is_left = 0;
		}
		else
			cont->is_left = 0;
	}
	else
		cont->is_left = -1;
	cont->nb_cmd = count_executable_nodes(tr);
	cont->exec = create_exec_tab(g, tr);
	cont->node_id = tr->id;
	return (cont);
}
