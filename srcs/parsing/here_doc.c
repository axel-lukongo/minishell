/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 17:49:48 by denissereno       #+#    #+#             */
/*   Updated: 2022/09/11 17:50:11 by denissereno      ###   ########.fr       */
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
		if (!ft_strncmp(stop, buf, ft_strlen(stop) + 1))
			return ;
		if (!quoted)
			buf = extract_env_var_call(buf, g);
		write(file, buf, ft_strlen(buf));
		write(file, "\n", 1);
	}
}

char	*read_file(int fd, t_alloc **alloc)
{
	int		read_val;
	char	*str;
	char	*tmp;
	char	buff[1084];

	str = NULL;
	read_val = 1;
	if (fd < 1)
		return (NULL);
	if (read(fd, 0, 0) == -1)
		return (NULL);
	while (read_val)
	{
		read_val = read(fd, buff, 1084);
		buff[read_val] = 0;
		tmp = str;
		str = ft_strjoin(tmp, buff, alloc);
	}
	if (str[0] == 0)
		return (NULL);
	return (str);
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
		g->last_return = 128 + WTERMSIG(status);
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
		g->last_return = WEXITSTATUS(status);
	close(file);
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
	signal(SIGQUIT, handle_signale_ctrl_c);
	signal(SIGINT, SIG_IGN);
	if (pid == 0)
	{
		signal(SIGINT, NULL);
		heredoc_loop(stop[0], file, g, (is_char(delim, '\'')
				|| is_char(delim, '"')));
		exit(0);
	}
	else
		ft_waitpid_heredoc(g, tmp, file, pid);
	open_file_heredoc(stop);
}
