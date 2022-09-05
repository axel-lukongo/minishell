/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 13:40:32 by alukongo          #+#    #+#             */
/*   Updated: 2022/08/31 16:10:38 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/pipex.h"

/**
 * @brief Get the cmd object ici je cherche la commande
 * 
 * @param paths 
 * @param cmd 
 * @return char* 
 */
static char	*get_cmd(char **paths, char *cmd)
{
	char	*tmp;
	char	*my_command;

	while (*paths)
	{
		tmp = ft_strjoin(*paths, "/");
		my_command = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(my_command, 0) == 0)
			return (my_command);
		free(my_command);
		paths++;
	}
	return (NULL);
}

static void	sub_dup2(int zero, int first)
{
	dup2(zero, 0);
	dup2(first, 1);
}


/**
 * @brief c'est ici que j'execute mes commande.
 * d'abord je fais un fork, puis je duplique le process,
 * pour dupliquer je verifie si je suis sur la premiere commade(if(p.idx) == 0)
 * sinon si je suis sur la derniere commande ou si je suis sur une des commades du milier
 * les formule que j'ai mis c'est pour me retrouver sur les bonne commande sinon.
 * si tous est correct je recupere la commande aisi que ces argument, puis je recupere le path
 * de la commade et je execute.
 * 
 * @param p 
 * @param argv 
 * @param envp 
 */

void	child(t_ppxb p, char **argv, char **envp)
{
	p.pid = fork();
	if (!p.pid)
	{
		// ls > test.txt > pipe.c | ls [0, 1, 2, 3, 4]; 0 -> >, 1 > >>; 2 <<; 3 < ; 4 |
		if (p.idx == 0)
			sub_dup2(p.infile, p.pipe[1]);
		// else if (p.idx == p.nbr_cmd - 1)
		// 	sub_dup2(p.pipe[2 * p.idx - 2], p.outfile);
		// else
		// 	sub_dup2(p.pipe[2 * p.idx - 2], p.pipe[2 * p.idx + 1]);
		printf("%d\n", p.redir_type[p.idx]);
		if (p.redir_type[p.idx] == 1)
			sub_dup2(p.infile, p.pipe[1]);
		else if (p.redir_type[p.idx] == 0)
			sub_dup2(p.pipe[2 * p.idx - 2], p.outfile);
		else if (p.redir_type[p.idx] == 4)
			sub_dup2(p.pipe[2 * p.idx - 2], p.pipe[2 * p.idx + 1]);
		close_pipes(&p);
		p.cmd_args = ft_split(argv[2 + p.here_doc + p.idx], ' ');
		p.cmd = get_cmd(p.cmd_paths, p.cmd_args[0]);
		if (!p.cmd)
		{
			msg_pipe(p.cmd_args[0]);
			free_child(&p);
			exit(1);
		}
		execve(p.cmd, p.cmd_args, envp);
	}
}
