/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 17:22:56 by denissereno       #+#    #+#             */
/*   Updated: 2022/09/12 12:50:18 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exit_utils(t_global *g, char **cmd)
{
	int	tmp;

	if (cmd[2] != NULL)
	{
		error_msg("exit", "too many arguments");
		g->last_return = 1;
	}
	else
	{
		clear_history();
		tmp = ft_atoi(cmd[1]) % 256;
		ft_malloc_clear(&g->alloc);
		exit(tmp);
	}
}

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
