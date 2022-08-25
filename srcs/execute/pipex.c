/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 13:49:14 by darian            #+#    #+#             */
/*   Updated: 2022/08/25 02:28:12 by alukongo         ###   ########.fr       */
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
	paths = ft_split(path_val, ':', g->alloc);
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

	res = ft_split(cmd, ' ', g->alloc);
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

void	exec (char *cmd, t_list *env, t_global *g)
{
	char	**args;
	char	*path;

	if (!cmd)
		exit(1);
	args = ft_split_quote(cmd, ' ', g->alloc);
	if (!is_char(args[0], '/'))
		path = valid_path(args[0], env, g);
	else
	{
		path = delete_path(args[0], &g->alloc);
		path = valid_path(path, env, g);
	}
	execve(path, args, g->char_env);
	write(STDERR, "sh: ", 4);
	write(STDERR, args[0], ft_strlen(args[0]));
	write(STDERR, ": command not found\n", 20);
	exit(127);
}

void	redir(t_exec *cmd, t_list *env, int fdin, t_global *g)
{
	pid_t		pid;
	int		fd[2];

	pipe(fd);
	pid = fork();
	if (pid)
	{
		close(fd[1]);
		dup2(fd[0], STDIN);
		waitpid(pid, NULL, 0);
	}
	else
	{
		close(fd[0]);
		dup2(fd[1], STDOUT);
		if (fdin == STDIN)
			exit(1);
		else
			exec(cmd->cmd, env, g);
	}
	close(fd[0]);
	close(fd[1]);
}

void	pipex(int n, t_exec **cmd, t_list *env, t_global *g)
{
	// il faut recoder pipex car elle execute pas les commandes en même temps.
	int	i;
	int	k;
	int	fd;

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
			exec(ft_strjoin("cat ", cmd[i + 1]->cmd, &g->alloc), env, g);
		}
		else
			fd = dup2(STDOUT, STDOUT);
		redir(cmd[i++], env, fd, g);
		k++;
	}
	exec(cmd[k]->cmd, env, g);
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
