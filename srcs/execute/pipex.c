/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 13:49:14 by darian            #+#    #+#             */
/*   Updated: 2022/09/05 17:59:28 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

# define STDIN 0
# define STDOUT 1
# define STDERR 2

# define INFILE 0
# define OUTFILE 1

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
		if(access(path_cmd, 0) == 0)
			return (path_cmd);
		i++;
	}
	return (NULL);
}

char	**split_cmd(char *cmd, t_global *g)
{
	char	**res;

	res = ft_split(cmd, ' ', &g->alloc);
	if (!res)
		return (NULL);
	return (res);
}

char	*delete_quote(char *str, t_alloc **alloc)
{
	size_t	i;
	size_t	j;
	char	*dest;
	t_vector2D	quote;
	t_vector2D	last_q;

	i = 0;
	j = 0;
	quote = (t_vector2D){0, 0};
	last_q = (t_vector2D){0, 0};
	printf("%s\n", str);
	dest = ft_malloc(sizeof(char) *( ft_strlen(str) + 1), alloc);
	if (!dest)
		return (NULL);
	while (str[i] != '\0')
	{
		if (str[i] == '"' && !is_backed((char *)str, i - 1) && !quote.x)
		{
			quote.y = (quote.y + 1) % 2;
			last_q.y = 0;
			if (quote.y == 0)
				last_q.y = 1;
		}
		else if (str[i] == '\'' && !quote.y)
		{
			quote.x = (quote.x + 1) % 2;
			last_q.x = 0;
			if (quote.x == 0)
				last_q.x = 1;
		}
		else if (str[i] == '\\')
		{
			if ((quote.x || quote.y) && str[i + 1] != '\\')
			{
				i += 1;
				dest[j++] = str[i + 1];
			}
			else
			{
				dest[j++] = str[i + 1];
				i+= 1;
			}
		}
		else if (str[i] == '"' && is_backed((char *)str, i - 1))
			dest[j++] = str[i];
		else if ((!(str[i] == '"' && (quote.y || last_q.y)) && !(str[i] == '\'' && (quote.x || last_q.x))) || (str[i] == '"' && quote.x) || (str[i] == '\'' && quote.y))
			dest[j++] = str[i];
		i++;
	}
	dest[j] = '\0';
	return (dest);
}

void	delete_all_quote(char **s, t_global *g)
{
	int	i;

	i = 0;
	while (s[i])
	{
		s[i] = delete_quote(s[i], &g->alloc);
		i++;
	}
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

char *ft_get_path(char **args, t_list *env, t_global *g)
{
	char *path;

	path = NULL;
	if (!is_char(args[0], '/'))
		path = valid_path(args[0], env, g);
	else if (!(ft_strncmp(args[0], "./", 2)) || (!(ft_strncmp(args[0], "../", 3))))
	{
		if(!(ft_strncmp(args[0], "./", 2)))
		{
			path = ft_strjoin(get_value_by_name(env ,"PWD"), "/", &g->alloc);
			path = ft_strjoin(path, args[0] + 2, &g->alloc);
		}
		else if(!(ft_strncmp(args[0], "../", 3)))
		{
			path = ft_strjoin(get_value_by_name(env ,"PWD"), "/", &g->alloc);
			path = ft_strjoin(path, args[0] + 3, &g->alloc);
		}
	}
	else
	{
		if (is_directory(args[0]))
		{
			error_msg(args[0], "is a directory");
			return (NULL);
		}
		path = delete_path(args[0], &g->alloc);
		path = valid_path(path, env, g);
	}
	return(path);
}

void	exec (char **args, t_list *env, t_global *g)
{
	char	*path;
	int		status;
	int		pid;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGQUIT, handle_signale_ctrl_c);
		if (!args || !args[0])
			exit(1);
		path = ft_get_path(args, env, g);
		execve(path, args, g->char_env);
		write(STDERR, "sh: ", 4);
		write(STDERR, args[0], ft_strlen(args[0]));
		write(STDERR, ": command not found\n", 20);
		exit(127);
	}
	else
	{
		waitpid(-1, &status, 0);
		if ((WIFSIGNALED(status)))
		{
			g->last_return = 128 + WTERMSIG(status);
			if (WTERMSIG(status) != 3)
				g->sig_exited = 1;
		}
		else
			g->last_return = WEXITSTATUS(status);
	}
}

void	pipex(int n, t_exec **cmd, t_list *env, t_global *g)
{
	int	i;
	int	k;
	int	fd;
	int	pid;
	(void)g;
	(void)env;

	pid = 0;
	i = 0;
	k = 0;
	fd = 1;
	while (i < n - 1)
	{
		if (cmd[i + 1]->redirect_last == GREAT)
		{
			fd = dup2(open(cmd[i + 1]->cmd, O_CREAT | O_WRONLY | O_TRUNC,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH), STDOUT);
			i++;
			continue;
		}
		else if (cmd[i + 1]->redirect_last == DGREAT)
		{
			fd = dup2(open(cmd[i + 1]->cmd, O_CREAT | O_WRONLY | O_APPEND,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH), STDOUT);
			i++;
			continue;
		}
		else if (cmd[i + 1] && cmd[i + 1]->redirect_last == LESS)
		{
			dup2(open("test.txt", O_RDONLY), STDIN);
			pid = fork();
			if (pid)
			{
				waitpid(pid, NULL, 0);
			}
			// else
				// exec(ft_strjoin("cat ", cmd[i + 1]->cmd, &g->alloc), env, g);
		}
		else
			fd = dup2(STDOUT, STDOUT);
		// printf("%d\n", i);
		// redir(cmd[i++], env, fd, g);
		k++;
	}
	// exec(cmd[k]->cmd, env, g);
	close(fd);
}

// PIPEX PAS BON CAR EXECUTE PAS EN MEME TEMPS

// if CMD > FILE. Dup2 STDOUT par FILE avec O_TRUNC et exec 
//		dup2(open("test.txt", O_CREAT | O_WRONLY | O_TRUNC,
//				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH), STDOUT);
//
// if CMD >> FILE. Dup2 STDOUT par FILE avec O_APPEND et exec 
//		dup2(open("test.txt", O_CREAT | O_WRONLY | O_APPEND,
//				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH), STDOUT);
// if CMD < FILE. Dup2 STDIN par FILE et O_RDONLY et exec la cmd.
//		dup2(open("test.txt", O_RDONLY), STDIN);
// if CMD << HERE_DOC. On verra.
