/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 17:49:48 by denissereno       #+#    #+#             */
/*   Updated: 2022/09/13 12:36:21 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	heredoc_loop(char *stop, int file, t_global *g, int quoted)
{
	char	*buf;

	if (!stop)
		return ;
	while (1)
	{
		buf = readline("heredoc> ");
		if (buf == NULL)
			g->error = 1;
		if (!ft_strncmp(stop, buf, ft_strlen(stop) + 1))
			return ;
		if (!quoted)
			buf = extract_env_var_call(buf, g);
		write(file, buf, ft_strlen(buf));
		write(file, "\n", 1);
	}
}

int	open_file_heredoc(char **files)
{
	int	i;
	int	error;
	int	fd;

	error = 0;
	i = 1;
	while (files[i])
	{
		fd = open(files[i], O_RDONLY, 0000644);
		if (fd == -1)
		{
			error_msg(files[i], "No such file or directory");
			error = 1;
		}
		else
			close(fd);
		i++;
	}
	return (error);
}

void	ft_waitpid_heredoc(t_global *g, char *tmp, int file, int pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if ((WIFSIGNALED(status)))
	{
		g_p->last_return = 128 + WTERMSIG(status);
		if (WTERMSIG(status) != 3)
		{
			printf("\n");
			close(file);
			open(tmp, O_CREAT | O_TRUNC);
			g->sig_exited = 1;
			close(file);
		}
	}
	else
		g_p->last_return = WEXITSTATUS(status);
	close(file);
}

void	hd_body(int file, t_global *g, char *str[2], char **stop)
{
	signal(SIGINT, NULL);
	signal(SIGQUIT, SIG_IGN);
	heredoc_loop(stop[0], file, g, (is_char(str[1], '\'')
			|| is_char(str[1], '"')));
	if (g->error)
	{
		close(file);
		open(str[0], O_CREAT | O_TRUNC);
		g->sig_exited = 1;
		close(file);
		g->error = 0;
	}
	exit(0);
}

void	here_doc_parser(char *delim, t_global *g, int tmpfile)
{
	int		file;
	char	**stop;
	char	*tmp;
	int		pid;

	tmp = ft_strjoin(".heredoc_tmp", ft_itoa(tmpfile, g->alloc), &g->alloc);
	stop = ft_split_quote(delim, ' ', g->alloc);
	if (!stop || !stop[0])
		return ;
	file = open(tmp, O_CREAT | O_WRONLY | O_TRUNC | O_RDONLY, 0000644);
	if (file < 0)
		msg_error("here_doc");
	pid = fork();
	signal_call();
	if (pid == 0)
		hd_body(file, g, (char *[2]){tmp, delim}, stop);
	else
	{
		signal(SIGQUIT, SIG_IGN);
		ft_waitpid_heredoc(g, tmp, file, pid);
	}
	close(file);
	open_file_heredoc(stop);
}
