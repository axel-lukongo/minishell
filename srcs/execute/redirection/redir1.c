/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 17:56:00 by denissereno       #+#    #+#             */
/*   Updated: 2022/09/13 11:18:44 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	parent_redir(t_global *g, t_tree *root, char *cmd, t_vector2D fdred)
{
	char	*tmp;
	int		file;

	if (root->type == DLESS)
	{
		tmp = ft_strjoin(".heredoc_tmp", ft_itoa(g->tmpfile, g->alloc),
				&g->alloc);
		file = open(tmp, O_RDONLY);
		if (!root->left)
			write_buffer(file);
		else
		{
			dup2(file, 0);
			exec_ast(root->left, g, cmd);
			dup2(fdred.x, fdred.y);
			close(fdred.x);
		}
	}
	else
	{
		dup2(fdred.x, fdred.y);
		close(fdred.x);
		exec_ast(root->left, g, cmd);
	}
	exit(g_p->last_return);
}

static void	exec_redirection(t_tree *root, t_vector2D fdred, t_global *g,
	char *cmd)
{
	int		child_status;

	child_status = 0;
	if (root->type == DLESS)
		g->hered = 1;
	if (root->left && !is_command(ft_split_quote(root->left->value, ' ',
				g->alloc)[0], g) && (root->left->type < LESS
		|| root->left->type > PIPE) && root->left->type != AND
			&& root->left->type != OR)
	{
		msg_pipe(root->left->value);
		return ;
	}
	if (fork() == 0)
		parent_redir(g, root, cmd, fdred);
	else
	{
		wait(&child_status);
		g_p->last_return = WEXITSTATUS(child_status);
	}
}

void	open_all_file_test(t_tree	*root, t_global *g)
{
	if (!root || root->type < LESS || root->type > DGREAT
		|| root->type == DLESS)
		return ;
	if (root->left && root->left->type >= LESS && root->left->type <= DGREAT
		&& root->left->type != DLESS)
		open_all_file_test(root->left, g);
	if (g->error == 1)
		return ;
	open_file_good_fd(root, g);
	return ;
}

t_token	**get_redir_tab(t_tree *node, t_global *g, t_tree *root)
{
	t_token	**tab;

	if (node->type == GREAT || node->type == DGREAT || node->type == DLESS)
		tab = get_last_redir_here(root, (t_vector3D){GREAT, DGREAT, DLESS},
				&g->alloc);
	else
		tab = get_last_redir(root, LESS, DLESS, &g->alloc);
	open_all_file_test(root, g);
	return (tab);
}

int	redirections(t_tree *root, t_global *g, char *cmd)
{
	int				fd;
	t_tree			*node;
	int				redirection;
	t_token			**tab;

	redirection = 1;
	node = root;
	tab = get_redir_tab(node, g, root);
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
	exec_redirection(root, (t_vector2D){fd, redirection}, g, cmd);
	return (0);
}
