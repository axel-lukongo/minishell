/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 13:50:49 by alukongo          #+#    #+#             */
/*   Updated: 2022/08/24 19:01:37 by alukongo         ###   ########.fr       */
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
			parent_free(pipex);
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
 * etape4: crée les pipe
 * etape5: cree les divers process et executer les cmd
 * etape6: fermer les pipe et attendre le process parent
 * 
 * @param argc 
 * @param argv 
 * @param envp 
 * @return int 
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
	pipex.pipe = (int *)malloc(sizeof(int) * pipex.nbr_pipe);
	if (!pipex.pipe)
		msg_error(ERR_PIPE);
	pipex.my_env_path = find_path_in_env(envp);
	pipex.cmd_paths = ft_split(pipex.my_env_path, ':');
	if (!pipex.cmd_paths)
		pipe_free(&pipex);
	creat_pipes(&pipex);
	pipex.idx = -1;
	while (++(pipex.idx) < pipex.nbr_cmd)
		child(pipex, argv, envp);
	close_pipes(&pipex);
	waitpid(-1, NULL, 0);
	parent_free(&pipex);
	return (0);
}
