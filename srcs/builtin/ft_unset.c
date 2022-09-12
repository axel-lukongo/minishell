/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 18:46:07 by alukongo          #+#    #+#             */
/*   Updated: 2022/09/12 11:58:56 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	unset_utils(char **cmd, int i)
{
	ft_putstr_fd("minishell: unset: `", 2);
	ft_putstr_fd(cmd[i], 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

void	my_unset(t_global *g, char **cmd)
{
	int		i;

	i = 0;
	while (cmd[++i])
	{
		if (cmd[i] && cmd[i][0] == '-')
		{
			ft_putstr_fd("minishell: unset: ", 2);
			ft_putchar_fd(cmd[i][0], 2);
			ft_putchar_fd(cmd[i][1], 2);
			ft_putstr_fd(": invalid option\n", 2);
			g->last_return = 2;
		}
		else if (cmd[i] && is_valid_identifier(cmd[i]) && !is_char(cmd[i], '='))
		{
			destroy_env_var(&g->env, cmd[i]);
			destroy_env_var(&g->export, cmd[i]);
			g->last_return = 0;
		}
		else
		{
			unset_utils(cmd, i);
			g->last_return = 1;
		}
	}
}
