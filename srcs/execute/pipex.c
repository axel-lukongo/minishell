/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 13:49:14 by darian            #+#    #+#             */
/*   Updated: 2022/09/11 16:02:19 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*valid_path(char *cmd, t_list *env, t_global *g)
{
	char	*path_val;
	char	**paths;
	char	*path_cmd;
	int		i;

	path_val = get_value_by_name(env, "PATH");
	paths = ft_split(path_val, ':', &g->alloc);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		path_cmd = ft_strjoin(paths[i], "/", &g->alloc);
		path_cmd = ft_strjoin(path_cmd, cmd, &g->alloc);
		if (!path_cmd)
			return (NULL);
		if (access(path_cmd, 0) == 0)
			return (path_cmd);
		i++;
	}
	return (NULL);
}

char	*delete_path(char *str, t_alloc **alloc)
{
	char		*new;
	t_vector2D	pos;

	new = ft_malloc(sizeof(char) * (ft_strlen(str) + 1), alloc);
	pos = (t_vector2D){0, 0};
	while (str[pos.x])
	{
		if (str[pos.x] == '/')
			pos.y = pos.x + 1;
		pos.x++;
	}
	pos.x = 0;
	while (str[pos.y])
	{
		new[pos.x++] = str[pos.y];
		pos.y++;
	}
	new[pos.x] = 0;
	return (new);
}

char	*ft_get_path_2(int *err, char **args, t_list *env, t_global *g)
{
	char	*path;

	path = NULL;
	if (is_directory(args[0]))
	{
		*err = 1;
		return (NULL);
	}
	if (!(ft_strncmp(args[0], "./", 2)))
	{
		path = ft_strjoin(get_value_by_name(env, "PWD"), "/", &g->alloc);
		path = ft_strjoin(path, args[0] + 2, &g->alloc);
	}
	else if (!(ft_strncmp(args[0], "../", 3)))
	{
		path = ft_strjoin(get_value_by_name(env, "PWD"), "/", &g->alloc);
		path = ft_strjoin(path, args[0] + 3, &g->alloc);
	}
	return (path);
}

char	*ft_get_path(char **args, t_list *env, t_global *g, int *err)
{
	char	*path;

	path = NULL;
	if (!is_char(args[0], '/'))
		path = valid_path(args[0], env, g);
	else if (!(ft_strncmp(args[0], "./", 2)) || (!(ft_strncmp(args[0], "../",
					3))))
		path = ft_get_path_2(err, args, env, g);
	else
	{
		if (is_directory(args[0]))
		{
			*err = 1;
			return (NULL);
		}
		path = delete_path(args[0], &g->alloc);
		path = valid_path(path, env, g);
	}
	return (path);
}

void	exec(char **args, t_list *env, t_global *g)
{
	char	*path;
	int		error;

	error = 0;
	if (!args || !args[0])
		exit(1);
	path = ft_get_path(args, env, g, &error);
	execve(path, args, g->char_env);
	if (error == 1)
		error_msg(args[0], "Is a directory");
	else
		error_msg(args[0], "command not found");
	exit(127);
}
