/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 13:47:22 by darian            #+#    #+#             */
/*   Updated: 2022/08/19 15:27:27 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	showTrunks(Trunk *p)
{
	if (p == NULL)
		return ;
	showTrunks(p->prev);
	printf(" %s", p->str);
}

void	printTree(t_tree *root, Trunk *prev, int isLeft)
{
	Trunk	*trunk;
	char	*prev_str;

	if (root == NULL)
	{
		return ;
	}
	prev_str = "  ";
	trunk = malloc(sizeof(*trunk));
	trunk->prev = prev;
	trunk->str = prev_str;
	printTree(root->right, trunk, 1);
	if (!prev)
		trunk->str = "———";
	else if (isLeft)
	{
		trunk->str = "  .———";
		prev_str = "   |";
	}
	else
	{
		trunk->str = "`———";
		prev->str = prev_str;
	}
	showTrunks(trunk);
	printf(" (%s)\n", root->value);
	if (prev)
		prev->str = prev_str;
	trunk->str = "   |";
	printTree(root->left, trunk, 0);
}

void	printtr(t_tree	*tr)
{
	if (!tr)
	{
		printf("tree empty\n");
		return ;
	}
	if (tr->left)
		printtr(tr->left);
	printf("%s ", tr->value);
	if (tr->right)
		printtr(tr->right);
}

void	print_tree_command_line(t_tree *tr)
{
	printtr(tr);
	printf("\n");
}
