/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/30 14:00:24 by denissereno       #+#    #+#             */
/*   Updated: 2022/09/05 18:58:13 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_command(char *cmd, t_global *g)
{
	int		i;
	char	**paths;

	i = 0;
	if (is_builtin(cmd))
		return (1);
	paths = ft_split(get_value_by_name(g->env, "PATH"), ':', &g->alloc);
	while (paths[i])
	{
		paths[i] = ft_strjoin(paths[i], "/", &g->alloc);
		paths[i] = ft_strjoin(paths[i], cmd, &g->alloc);
		if (!access(paths[i], 0))
			return (1);
		i++;
	}
	return (0);
}

 void	concat_cmd(char **cmd, t_tree *root, t_global *g)
{
	char	**test;

	test = ft_split_quote(root->left->value,' ', g->alloc);
	if (test[1])
	{
		// rev_tab((void **)test);
		int	i;
		i = 0;
		while (test[i])
		{
			*cmd = ft_strjoin(*cmd, " ", &g->alloc);
			*cmd = ft_strjoin(*cmd, test[i], &g->alloc);
			i++;
		}
	}
	else
	{
		*cmd = ft_strjoin(*cmd, " ", &g->alloc);
		*cmd = ft_strjoin(*cmd, root->left->value, &g->alloc);
	}
}

static void	exec_redirection(t_tree *root, int fd, int redirection, t_global *g, char *cmd)
{
	int				child_status;

	child_status = 0;
	if (root->left && !is_command(ft_split_quote(root->left->value, ' ', g->alloc)[0], g) && (root->left->type < LESS
		|| root->left->type > PIPE ) && root->left->type != AND && root->left->type != OR)
	{
		msg_pipe(root->left->value);
		return ;
	}
	if (fork() == 0)
	{
		// (void)redirection;
		dup2(fd, redirection);
		close(fd);
		exec_ast(root->left, g, cmd);
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
	if (!tab)
		return (0);
	while (tab[i])
		i++;
	return (i);
}

static int			fd_great_redir(char **tab, t_global *g)
{
	int				i;
	int				fd;

	i = 0;
	// while (tab[i])
	// {
		fd = open(tab[0], O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (fd == -1)
		{
			error_msg(tab[0], "No such file or directory");
			g->last_return = 1;
			return (-1);
		}
		i++;
	// }
	return (fd);
}

static int			fd_dgreat_redir(char **tab, t_global *g)
{
	int				i;
	int				fd;

	i = 0;
	while (tab[i])
	{
		fd = open(tab[i], O_CREAT | O_RDWR | O_APPEND, 0644);
		if (fd == -1)
		{
			error_msg(tab[i], "No such file or directory");
			g->last_return = 1;
			return (-1);
		}
		i++;
	}
	return (fd);
}

static int			fd_less_redir(char **tab, t_global *g)
{
	int				i;
	int				fd;
	// char			*join;

	i = 0;
	while (tab[i])
	{
		fd = open(tab[i], O_RDONLY);
		if (fd == -1)
		{
			error_msg(tab[i], "No such file or directory");
			g->last_return = 1;
			return (-1);
		}
		i++;
	}
	return (fd);
}


int open_file(int type, t_token **tab, t_global *g)
{
	int				fd;
	int				i;

	fd = 0;
	i = 0;
	(void)type;
	while(tab[i])
	{
		if (tab[i]->type == GREAT)
			fd = fd_great_redir(ft_split_quote(tab[i]->str, ' ', g->alloc), g);
		else if (tab[i]->type == DGREAT)
			fd = fd_dgreat_redir(ft_split_quote(tab[i]->str, ' ', g->alloc), g);
		else if (tab[i]->type == LESS)
			fd = fd_less_redir(ft_split_quote(tab[i]->str, ' ', g->alloc), g);
		i++;
	}
	return (fd);
}

// char	**get_files_redir(c)
// {
	
// }

int	size_tree_type(t_tree *root, int type1, int type2)
{
	int	i;

	i = 0;
	while (root->parent && (root->parent->type == type1 || root->parent->type == type2))
	{
		i++;
		root = root->parent;
	}
	return (i);
}

t_token	**get_last_redir(t_tree *root, int type1, int type2, t_alloc **alloc)
{
	t_token	**tab;
	int		i;

	if ((!root->parent || (root->parent->type != type1 && root->parent->type
	!= type2)) && (root->type == type1 || root->type == type2))
	{
		tab = ft_malloc(sizeof(t_token *) * 2, alloc);
		tab[0] = ft_malloc(sizeof(t_token *) * 2, alloc);
		if (root->right->type == WILDBACK || root->right->type == WILDCARD ||
		root->right->type == WILDENV || root->right->type == WILDENVBACK)
			tab[0]->str = ft_strdup(root->right->value, alloc);
		else
			tab[0]->str = ft_split_quote(root->right->value, ' ', *alloc)[0];
		tab[0]->type = root->type;
		tab[1] = NULL;
		return (tab);
	}
	else if (root->parent && (root->parent->type == type1 || root->parent->type == type2))
	{
		i = 0;
		tab = ft_malloc(sizeof(t_token *) * (1000), alloc);
		while (root->parent && (root->parent->type == type1 || root->parent->type == type2))
		{
			tab[i] = ft_malloc(sizeof(t_token *) * 2, alloc);
			if (root->right->type == WILDBACK || root->right->type == WILDCARD ||
			root->right->type == WILDENV || root->right->type == WILDENVBACK)
				tab[i]->str = root->right->value;
			else
				tab[i]->str = ft_split_quote(root->right->value, ' ', *alloc)[0];
			tab[i]->type = root->type;
			root = root->parent;
			i++;
		}
		tab[i] = ft_malloc(sizeof(t_token *) * 2, alloc);
		if (root->right->type == WILDBACK || root->right->type == WILDCARD ||
		root->right->type == WILDENV || root->right->type == WILDENVBACK)
			tab[i]->str = root->right->value;
		else
		{
			tab[i]->str = ft_split_quote(root->right->value, ' ', *alloc)[0];
		}
		tab[i]->type = root->type;
		i++;
		tab[i] = NULL;
		return (tab);
	}
	else
		return (NULL);
}

// Creer un tableau de token a la place, ajouter le type de redir a chaque file pour ouvrir de la bonne maniere lors de la redir.

void	print_tab(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i])
	{
		printf("%s\n", tab[i]);
		i++;
	}
}

t_token **rev_token_tab(t_token **tab)
{
	int		i;
	int		j;
	void	*tmp;

	i = 0;
	j = tab_size((void **)tab) - 1;
	while (i <= j)
	{
		tmp = tab[i];
		tab[i] = tab[j];
		tab[j] = tmp;
		i++;
		j--;
	}
	return (tab);
}

char	*get_cmd_tree(t_tree *root, int type1, int type2, t_alloc **alloc)
{
	char	*res;

	res = NULL;
	if ((!root->parent || (root->parent->type != type1 && root->parent->type
	!= type2)) && (root->type == type1 || root->type == type2))
	{
		if (root->left && ((root->left->type >= 0 && root->left->type <= 3)
			|| (root->left->type >= 20 && root->left->type <= 24)))
		{
			res = ft_strjoin(res, " ", alloc);
			res = ft_strjoin(res, root->left->value, alloc);
		}
		if (root->right && ft_split_first(root->right->value, ' ', alloc)[1])
		{
			res = ft_strjoin(res, " ", alloc);
			res = ft_strjoin(res, ft_split_first(root->right->value, ' ', alloc)[1], alloc);
		}
		return (res);
	}
	else if (root->parent && (root->parent->type == type1 || root->parent->type == type2))
	{
		while (root->parent && (root->parent->type == type1 || root->parent->type == type2))
		{
			if (root->left && ((root->left->type >= 0 && root->left->type <= 3)
			|| (root->left->type >= 20 && root->left->type <= 24)))
			{
				res = ft_strjoin(res, " ", alloc);
				res = ft_strjoin(res, root->left->value, alloc);
			}
			if (root->right && ft_split_first(root->right->value, ' ', alloc)[1])
			{
				res = ft_strjoin(res, " ", alloc);
				res = ft_strjoin(res, ft_split_first(root->right->value, ' ', alloc)[1], alloc);
			}
			root = root->parent;
		}
		res = ft_strjoin(res, " ", alloc);
		res = ft_strjoin(res, ft_split_first(root->right->value, ' ', alloc)[1], alloc);
		return (res);
	}
	else
		return (NULL);
}

void	open_all_file_test(t_tree	*root, t_global *g)
{
	if (!root || root->type < LESS ||  root->type > DGREAT || root->type == DLESS)
		return ;
	if (root->left && root->left->type >= LESS && root->left->type <= DGREAT && root->left->type != DLESS)
		open_all_file_test(root->left, g);
	if (g->error == 1)
		return ;
	if (root->type == GREAT)
	{
		if (fd_great_redir(ft_split_quote(root->right->value, ' ', g->alloc), g) == -1)
			g->error = 1;
	}
	else if (root->type == DGREAT)
	{
		if (fd_dgreat_redir(ft_split_quote(root->right->value, ' ', g->alloc), g) == -1)
			g->error = 1;
	}
	else if (root->type == LESS)
	{
		if(fd_less_redir(ft_split_quote(root->right->value, ' ', g->alloc), g) == -1)
			g->error = 1;
	}
	return ;
}

int				redirections(t_tree *root, t_global *g, char *cmd)
{
	int				fd;
	t_tree			*node;
	int				redirection;
	t_token			**tab;

	redirection = 1;
	node = root;
	if (node->type == GREAT || node->type == DGREAT)
		tab = get_last_redir(root, GREAT, DGREAT, &g->alloc);
	else
		tab = get_last_redir(root, LESS, DLESS, &g->alloc);
	open_all_file_test(root, g);
	if (g->error == 1)
	{
		g->error = 0;
		return (-1);
	}
	if (node->type == LESS)
		redirection = 0;
	fd = open_file(node->type, tab, g);
	cmd = get_cmd_tree(root, GREAT, DGREAT, &g->alloc);
	if (fd == -1)
		return (-1);
	exec_redirection(root, fd, redirection, g, cmd);
	return (0);
}

void	rev_tab(void **tab)
{
	int		i;
	int		j;
	void	*tmp;

	i = 0;
	j = tab_size(tab) - 1;
	while (i <= j)
	{
		tmp = tab[i];
		tab[i] = tab[j];
		tab[j] = tmp;
		i++;
		j--;
	}
}

static void			exec_cmd(t_tree *node, t_global *g, char *cmd)
{
	char	**args;
	if (cmd)
	{
		args = ft_split_quote(cmd, ' ', g->alloc);
		if (is_builtin(args[0]))
			execute_builtin(g, args);
		else
			exec(args, g->env, g);
	}
	else
	{
		if (node->value && is_builtin(ft_split_quote(node->value, ' ', g->alloc)[0]))
			execute_builtin(g, ft_split_quote(node->value, ' ', g->alloc));
		else if (node->value)
		{
			
			exec(ft_split_quote(node->value, ' ', g->alloc), g->env, g);
		}
	}
}

void				pipeline(t_tree *root, int count, int backup_fd, t_global *g, char *cmd)
{
	int				fd[2];
	int				pid;

	if (!root)
		return ;
	if ((root->parent && root->parent->left && root->parent->left->type == PIPE) || root->type == PIPE)
		pipe(fd);
	if ((pid = fork()) == 0)
	{
		if (count)
			dup2(fd[0], 0);
		if (count != 1)
			dup2(backup_fd, 0);
		if ((root->parent && root->parent->left && root->parent->left->type == PIPE) || root->type == PIPE)
		{
			close(fd[0]);
			dup2(fd[1], 1);
		}
		if (count)
			exec_ast(root->left, g, cmd);
		else
			exec_ast(root, g, cmd);
		exit(g->last_return);
	}
	if ((root->parent && root->parent->left && root->parent->left->type == PIPE) || root->type == PIPE)
		close(fd[1]);
	pipeline(root->right, count - 1, fd[0], g, cmd);
	waitpid(-1, NULL, 0);
}

void	exec_and(t_tree *node, t_global *g, char *cmd)
{
	exec_ast(node->left, g, cmd);
	if (g->last_return == 0)
		exec_ast(node->right, g, cmd);
}

void	exec_or(t_tree *node, t_global *g, char *cmd)
{
	exec_ast(node->left, g, cmd);
	if (g->last_return != 0)
		exec_ast(node->right, g, cmd);
}

void	exit_sig(int sig)
{
	(void)sig;
	exit(errno % 256);
}

void	heredoc_loop(t_tree *node, int file)
{
	char	*buf;

	signal(SIGINT, exit_sig);
	signal(SIGQUIT, exit_sig);
	while (1)
	{
		buf = readline("heredoc> ");
		if (!ft_strncmp(node->right->value, buf, ft_strlen(node->right->value) + 1))
			return ;
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

void	here_doc(t_tree *node, t_global *g, char *cmd)
{
	int		file;
	int		fd[2];
	int		pid;

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
		dup2(file, 0);
		dup2(fd[1], 1);
		exec_cmd(node->left, g, cmd);
		close(fd[1]);
		close(fd[0]);
		close(file);
	}
	else
		waitpid(pid, NULL, 0);
}

void	exec_ast(t_tree *ast, t_global *g, char *cmd)
{
	t_tree *node;

	node = ast;
	if (ast == 0)
		return ;
	if (node->type == AND)
		exec_and(node, g, cmd);
	else if (node->type == OR)
		exec_or(node, g, cmd);
	else if ((node->type >= 0 &&  node->type <= 3)
		|| (node->type >= 20 &&  node->type <= 24) || node->type == WILDCARD)
		exec_cmd(node, g, cmd);
	else if (node->type == DLESS)
		here_doc(node, g, cmd);
	else if (node->type >= 9 && node->type <= 12)
		redirections(node, g, cmd);
	else if (node->type == PIPE)
		pipeline(node, 1, -1, g, cmd);
}