/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 13:40:32 by alukongo          #+#    #+#             */
/*   Updated: 2022/08/26 19:24:58 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Get the cmd object ici je cherche la commande
 * 
 * @param paths 
 * @param cmd 
 * @return char* 
 */
static char	*get_cmd(char **paths, char *cmd, t_alloc **alloc)
{
	char	*tmp;
	char	*my_command;

	while (*paths)
	{
		tmp = ft_strjoin(*paths, "/", alloc);
		my_command = ft_strjoin(tmp, cmd, alloc);
		if (access(my_command, 0) == 0)
			return (my_command);
		paths++;
	}
	return (NULL);
}

// static void	sub_dup2(int zero, int first)
// {
// 	dup2(zero, 0);
// 	dup2(first, 1);
// }

static void	redir_right(int zero, char *file)
{
	int	fd;

	dup2(zero, 0);
	fd = dup2(open(file, O_CREAT | O_WRONLY | O_TRUNC,
		S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH), 1);
	dup2(fd, 1);
}

/**
 * @brief c'est ici que j'execute mes commande.
 * d'abord je fais un fork, puis je duplique le process,
 * pour dupliquer je verifie si je suis sur la premiere commade(if(p.idx) == 0)
 * sinon si je suis sur la derniere commande ou si je suis sur une des commades du milierm
 * les formule que j'ai mis c'est pour me retrouver sur les bonne commande sinon.
 * si tous est correct je recupere la commande aisi que ces argument, puis je recupere le path
 * de la commade et je execute.
 * 
 * @param p 
 * @param argv 
 * @param envp 
 */

void	child(t_ppxb p, t_global *g)
{
	p.pid = fork();
	if (!p.pid)
	{
		// ls > test.txt > pipe.c | ls [0, 1, 2, 3, 4]; 0 -> >, 1 > >>; 2 <<; 3 < ; 4 |
		// if (p.idx == 0)
		// 	sub_dup2(p.infile, p.pipe[1]);
		// else if (p.idx == p.nbr_cmd - 1)
		// 	sub_dup2(p.pipe[2 * p.idx - 2], p.outfile);
		// else
		// 	sub_dup2(p.pipe[2 * p.idx - 2], p.pipe[2 * p.idx + 1]);

		if (p.exec[p.idx + 1] && p.exec[p.idx + 1]->redirect_last == GREAT) // > 
			redir_right(p.pipe[2 * p.idx - 2], p.exec[p.idx + 1]->cmd);
		// else if (p.redir_type[p.idx] == 1) // > ctrl C (faux here_doc)
		// 	sub_dup2(p.infile, p.pipe[1]);
		// else if (p.redir_type[p.idx] == 0) // <
		// 	sub_dup2(p.pipe[2 * p.idx - 2], p.outfile);
		// else if (p.exec[p.idx + 1] && p.exec[p.idx + 1]->redirect_last == PIPE) // |
		// 	sub_dup2(p.pipe[2 * p.idx - 2], p.pipe[2 * p.idx + 1]);
		// else if (p.exec[p.idx + 1] && p.exec[p.idx + 1]->redirect_last == DGREAT) // >>
		// 	sub_dup2(p.pipe[2 * p.idx - 2], p.pipe[2 * p.idx + 1]);
		// else if (p.exec[p.idx + 1] && p.exec[p.idx + 1]->redirect_last == DLESS) // <<
		// 	sub_dup2(p.pipe[2 * p.idx - 2], p.pipe[2 * p.idx + 1]);
		// else
		// 	sub_dup2(p.pipe[2 * p.idx - 2], p.pipe[2 * p.idx + 1]);
		close_pipes(&p);
		p.cmd_args = ft_split(p.exec[p.idx]->cmd, ' ', g->alloc);
		p.cmd = get_cmd(p.cmd_paths, p.cmd_args[0], &g->alloc);
		if (!p.cmd)
		{
			msg_pipe(p.cmd_args[0]);
			free_child(&p);
			exit(1);
		}
		execve(p.cmd, p.cmd_args, g->char_env);
	}
}
