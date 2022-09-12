/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 17:58:01 by denissereno       #+#    #+#             */
/*   Updated: 2022/09/12 19:06:50 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	fd_great_redir(char **tab, t_global *g)
{
	int				i;
	int				fd;

	i = 0;
	while (tab[i])
	{
		fd = open(tab[0], O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (fd == -1)
		{
			error_msg(tab[0], "No such file or directory");
			g->last_return = 1;
			return (-1);
		}
		if (tab[i + 1] != NULL)
			close (fd);
		i++;
	}
	return (fd);
}

static int	fd_dgreat_redir(char **tab, t_global *g)
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
		if (tab[i + 1] != NULL)
			close (fd);
		i++;
	}
	return (fd);
}

static int	fd_less_redir(char **tab, t_global *g)
{
	int				i;
	int				fd;

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
		if (tab[i + 1] != NULL)
			close (fd);
		i++;
	}
	return (fd);
}

int	open_file(int type, t_token **tab, t_global *g)
{
	int				fd;
	int				i;

	fd = 0;
	i = 0;
	(void)type;
	while (tab && tab[i])
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

void	open_file_good_fd(t_tree *root, t_global *g)
{
	if (root->type == GREAT)
	{
		if (fd_great_redir(ft_split_quote(root->right->value, ' ', g->alloc),
				g) == -1)
			g->error = 1;
	}
	else if (root->type == DGREAT)
	{
		if (fd_dgreat_redir(ft_split_quote(root->right->value, ' ', g->alloc),
				g) == -1)
			g->error = 1;
	}
	else if (root->type == LESS)
	{
		if (fd_less_redir(ft_split_quote(root->right->value, ' ', g->alloc),
				g) == -1)
			g->error = 1;
	}
}
