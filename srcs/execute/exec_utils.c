/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 17:22:56 by denissereno       #+#    #+#             */
/*   Updated: 2022/09/11 11:40:01 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**convert_tree_to_cmd(t_tree	*tr, t_global *g)
{
	char	**cmd;

	if (!tr)
		return (NULL);
	cmd = ft_malloc(sizeof(char *) * 3, &g->alloc);
	if (tr->left)
		cmd[0] = tr->left->value;
	if (tr->right)
		cmd[1] = tr->right->value;
	cmd[2] = NULL;
	return (cmd);
}

int	is_directory(char *str)
{
	if (!opendir(str) && !access(str, 0))
		return (0);
	return (1);
}
