/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 15:11:11 by denissereno       #+#    #+#             */
/*   Updated: 2022/09/12 18:35:16 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_char(char *str, char c)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

void	error_msg_double(char	*arg1, char *arg2, char *message)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(arg1, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(arg2, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(message, 2);
	ft_putstr_fd("\n", 2);
}

void	message_pwd(void)
{
	ft_putstr_fd("minishell: pwd: error retrieving current", 2);
	ft_putstr_fd(" directory: getcwd: cannot access parent ", 2);
	ft_putstr_fd("directories: No suck file or directory\n", 2);
}

