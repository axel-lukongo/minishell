/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 13:50:49 by alukongo          #+#    #+#             */
/*   Updated: 2022/08/26 19:24:06 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	creat_pipes(t_ppxb *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->nbr_cmd - 1)
	{
		if (pipe(pipex->pipe + 2 * i) < 0)
			free_parent(pipex);
		i++;
	}
}

void	close_pipes(t_ppxb *pipex)
{
	int	i;

	i = 0;
	while (i < (pipex->nbr_pipe))
	{
		close(pipex->pipe[i]);
		i++;
	}
}

/**
 * @brief etape1: recuperer les fd du premier et dernier fichier
 * etape2: recuperer le nombre de pipe necessaire aisi que le nb de commande
 * etape3: recuperer le path dans l'env et le split
 * etape4: créer les pipes
 * etape5: creer les divers process et executer les cmd
 * etape6: fermer les pipe et attendre le process parent
 * 
 * @param argc 
 * @param argv 
 * @param envp 
 * @return int 
 */

/*
0 = >
1 = <
2 = >>
3 = <<
4 = |
*/

int	count_cmd(t_exec **cmd)
{
	int		i;
	int		n;

	n = 0;
	i = 0;
	while (cmd[i])
	{
		if (cmd[i]->redirect_last == -1)
			n++;
		if (cmd[i]->redirect_last == PIPE)
			n++;
		i++;
	}
	return (n);
}

int	count_pipe(t_exec **cmd)
{
	int		i;
	int		n;

	n = 0;
	i = 0;
	while (cmd[i])
	{
		if (cmd[i]->redirect_last == PIPE)
			n++;
		i++;
	}
	return (n);
}

int	ft_pipex(int argc, t_exec **cmd, t_global *g)
{
	t_ppxb	pipex;

	// if (argc < args_in(argv[1], &pipex))
	// 	return (msg(ERR_INPUT));
	// init_infile(argv, &pipex);
	// init_outfile(argv[argc - 1], &pipex);
	(void)argc;
	printf("%d\n", argc);
	pipex.nbr_cmd = count_cmd(cmd);
	pipex.nbr_pipe = count_pipe(cmd);
	pipex.pipe = (int *)ft_malloc(sizeof(int) * pipex.nbr_pipe, &g->alloc);
	pipex.exec = cmd;
	if (!pipex.pipe)
		msg_error(ERR_PIPE);
	pipex.my_env_path = get_value_by_name(g->env, "PATH");
	pipex.cmd_paths = ft_split(pipex.my_env_path, ':', g->alloc);
	if (!pipex.cmd_paths)
		free_pipe(&pipex);
	creat_pipes(&pipex);
	pipex.idx = -1;
	while (++(pipex.idx) < pipex.nbr_cmd)
		child(pipex, g);
	close_pipes(&pipex);
	waitpid(-1, NULL, 0);
	free_parent(&pipex);
	return (0);
}
