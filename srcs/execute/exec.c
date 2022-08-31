/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/30 14:00:24 by denissereno       #+#    #+#             */
/*   Updated: 2022/08/30 19:18:04 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void			exec_redirection(t_tree *root, int fd, int redirection, t_global *g)
{
	int				child_status;

	child_status = 0;
	if (fork() == 0)
	{
		dup2(fd, redirection);
		close(fd);
		exec_ast(root->left, g);
		exit(g->last_return);
	}
	else
	{
		wait(&child_status);
		g->last_return = WEXITSTATUS(child_status);
	}
}

int	tab_size(void **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

static int			*fd_great_redir(char **tab, t_global *g)
{
	int				i;
	int				*fd;
	char			*join;

	i = 0;
	fd = ft_malloc(sizeof(int) * (tab_size((void **)tab) + 1), &g->alloc);
	while (tab[i])
	{
		fd[i] = open(tab[i], O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (fd[i] == -1)
		{
			join = ft_strjoin(tab[i], ": ", &g->alloc);
			printf("minishell: %s", join);
			ft_putstr_fd("\n", 2);
			g->last_return = 1;
			free(join);
			return (fd);
		}
		i++;
	}
	return (fd);
}

static int			*fd_dgreat_redir(char **tab, t_global *g)
{
	int				i;
	int				*fd;
	char			*join;

	i = 0;
	fd = ft_malloc(sizeof(int) * (tab_size((void **)tab) + 1), &g->alloc);
	while (tab[i])
	{
		fd[i] = open(tab[i], O_CREAT | O_RDWR | O_APPEND, 0644);
		if (fd[i] == -1)
		{
			join = ft_strjoin(tab[i], ": ", &g->alloc);
			printf("minishell: %s", join);
			ft_putstr_fd("\n", 2);
			g->last_return = 1;
			free(join);
			return (fd);
		}
		i++;
	}
	return (fd);
}

static int			*fd_less_redir(char **tab, t_global *g)
{
	int				i;
	int				*fd;
	char			*join;

	i = 0;
	fd = ft_malloc(sizeof(int) * (tab_size((void **)tab) + 1), &g->alloc);
	while (tab[i])
	{
		fd[i] = open(tab[i], O_RDONLY);
		if (fd[i] == -1)
		{
			join = ft_strjoin(tab[i], ": ", &g->alloc);
			printf("minishell: %s", join);
			ft_putstr_fd("\n", 2);
			g->last_return = 1;
			free(join);
			return (fd);
		}
		i++;
	}
	return (fd);
}


int *open_file(int type, char **file_name, t_global *g)
{
	int				*fd;

	fd = ft_malloc(sizeof(int) * (tab_size((void **)file_name) + 1), &g->alloc);
	if (type == GREAT)
		fd = fd_great_redir(file_name, g);
	else if (type == DGREAT)
		fd = fd_dgreat_redir(file_name, g);
	else if (type == LESS)
		fd = fd_less_redir(file_name, g);
	fd[tab_size((void **)file_name)] = -2;
	return (fd);
}

void				redirections(t_tree *root, t_global *g)
{
	int				*fd;
	t_tree			*node;
	int				redirection;
	char			**tab;
	int				i;

	redirection = 1;
	tab = ft_split(root->right->value, ' ', &g->alloc);
	node = root;
	if (node->type == LESS)
		redirection = 0;
	fd = open_file(node->type, tab, g);
	i = 0;
	while (fd[i] != -2)
	{
		if (fd[i] == -1)
			return ;
		exec_redirection(root, fd[i], redirection, g);
		i++;
	}
}

static void			exec_cmd(t_tree *node, t_global *g)
{
    if (is_builtin(node->value))
        execute_builtin(g, ft_split(node->value, ' ', &g->alloc));
	else
        exec(node->value, g->env, g);
}

void				pipeline(t_tree *root, int count, int backup_fd, t_global *g)
{
	int				fd[2];
	int				pid;

	if (!root)
		return ;
	pipe(fd);
	if ((pid = fork()) == 0)
	{
		dup2(backup_fd, 0);
		if (count)
			dup2(fd[1], 1);
		close(fd[0]);
		if (count)
			exec_ast(root->left, g);
		else
			exec_ast(root, g);
		exit(g->last_return);
	}
	close(fd[1]);
	pipeline(root->right, count - 1, fd[0], g);
	pid = wait(NULL);
}

void	exec_and(t_tree *node, t_global *g)
{
	exec_ast(node->left, g);
	exec_ast(node->right, g);
}

void	exec_or(t_tree *node, t_global *g)
{
	exec_ast(node->left, g);
	if (g->last_return != 0)
		exec_ast(node->right, g);
}

void	test(int sig)
{
	(void)sig;
	exit(errno);
}

void	heredoc_loop(t_tree *node, int file)
{
	char	*buf;

	signal(SIGINT, test);
	while (1)
	{
		buf = readline("heredoc> ");
		if (!ft_strncmp(node->right->value, buf, ft_strlen(node->right->value) + 1))
		{
			printf("%s\n", buf);
			return ;
		}
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

void	here_doc(t_tree *node, t_global *g)
{
	int		file;
	int		fd[2];
	char	*buffer;
	int		pid;

	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		file = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC | O_RDONLY,
			0000644); 
		if (file < 0)
			msg_error("here_doc");
		heredoc_loop(node, file);
		close(file);
		file = open(".heredoc_tmp", O_RDONLY);
		buffer = read_file(file, &g->alloc);
		write(fd[1], buffer, ft_strlen(buffer)); // a completer
		exec_cmd(node->left, g);
		close(file);
	}
	else
		waitpid(-1, NULL, 0);
}

void	exec_ast(t_tree *ast, t_global *g)
{
	t_tree *node;

	node = ast;
	if (ast == 0)
		return ;
	if (node->type == AND)
		exec_and(node, g);
	else if (node->type == OR)
		exec_or(node, g);
	else if ((node->type >= 0 &&  node->type <= 3)
		|| (node->type >= 20 &&  node->type <= 24) || node->type == WILDCARD)
		exec_cmd(node, g);
	else if (node->type == DLESS)
		here_doc(node, g);
	else if (node->type >= 9 && node->type <= 12)
		redirections(node, g);
	else if (node->type == PIPE)
		pipeline(node, 1, 0, g);
}