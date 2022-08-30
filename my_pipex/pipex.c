/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 13:50:49 by alukongo          #+#    #+#             */
/*   Updated: 2022/08/30 11:09:57 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/pipex.h"

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

void	init_redir_type(t_ppxb *pipex)
{
	pipex->redir_type = malloc(sizeof(int) * pipex->nbr_pipe);
	pipex->redir_type[0] = 1;
	for (int i = 1; i < pipex->nbr_pipe - 1; i++)
		pipex->redir_type[i] = 4;
	pipex->redir_type[pipex->nbr_pipe - 1] = 0;
}
/*
0 = >
1 = <
2 = >>
3 = <<
4 = |
*/

int	main(int argc, char **argv, char *envp[])
{
	t_ppxb	pipex;

	if (argc < args_in(argv[1], &pipex))
		return (msg(ERR_INPUT));
	init_infile(argv, &pipex);
	init_outfile(argv[argc - 1], &pipex);
	pipex.nbr_cmd = argc - 3 - pipex.here_doc;
	pipex.nbr_pipe = 2 * (pipex.nbr_cmd - 1);
	printf("-> %d\n", pipex.nbr_pipe);
	pipex.pipe = (int *)malloc(sizeof(int) * pipex.nbr_pipe);
	init_redir_type(&pipex);
	if (!pipex.pipe)
		msg_error(ERR_PIPE);
	pipex.my_env_path = find_path_in_env(envp);
	pipex.cmd_paths = ft_split(pipex.my_env_path, ':');
	if (!pipex.cmd_paths)
		free_pipe(&pipex);
	creat_pipes(&pipex);
	pipex.idx = -1;
	while (++(pipex.idx) < pipex.nbr_cmd)
		child(pipex, argv, envp);
	close_pipes(&pipex);
	waitpid(-1, NULL, 0);
	free_parent(&pipex);
	return (0);
}
