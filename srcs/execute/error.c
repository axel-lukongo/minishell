/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 13:40:39 by alukongo          #+#    #+#             */
/*   Updated: 2022/09/11 17:48:36 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	msg_pipe(char *arg)
{
	write(2, "Command not found: ", ft_strlen("Command not found: "));
	write(2, arg, ft_strlen(arg));
	write(2, "\n", 1);
}

void	msg_error(char *err)
{
	perror(err);
	exit (1);
}
