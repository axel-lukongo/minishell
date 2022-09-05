/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/11 20:07:36 by alukongo          #+#    #+#             */
/*   Updated: 2022/09/04 13:44:01 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_builtin(char *str)
{
	if	(!ft_strcmp(str, "echo") || !ft_strcmp(str, "export") || !ft_strcmp(str, "cd")
		|| !ft_strcmp(str, "unset") || !ft_strcmp(str, "env") || !ft_strcmp(str, "exit")
		|| !ft_strcmp(str, "dirs") || !ft_strcmp(str, "pwd"))
			return (1);
	return (0);
}

int	count_char(char *str, int ch)
{
	int	n;
	int	i;

	i = 0;
	n = 0;
	while	(str[i])
	{
		if	(str[i] == ch)
			n++;
		i++;
	}
	return (n);
}

char	*del_last_path(t_global *g, char *path)
{
	int		sl;
	int		i;
	char	*new;

	sl = count_char(path, '/');
	i = 0;
	new = ft_malloc(sizeof(char) * ft_strlen(path) + 1, &g->alloc);
	while	(path[i] && sl != 0)
	{
		if	(path[i] == '/')
			sl--;
		if	(sl == 0 && path[i] == '/')
			break ;
		new[i] = path[i];
		i++;
	}
	new[i] = 0;
	return (new);
}

int	dir_change_stack(char *str)
{
	int	i;

	i = 1;
	if (!str)
		return (0);
	if (str[0] != '-')
		return (0);
	if (str[0] == '-')
		return (1);
	while (str[i] >= '0' && str[i] <= '9')
		i++;
	if (str[i] != 0)
		return (0);
	return (1);
}

int	is_allowed_var(char *var)
{
	int	i;

	i = 0;
	while	(var[i])
	{
		if	(!is_shell_char_var_allowed(var[i]))
			return (0);
		i++;
	}
	return	(-1);
}
