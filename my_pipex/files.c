/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 13:40:45 by alukongo          #+#    #+#             */
/*   Updated: 2022/08/24 14:33:27 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/pipex.h"

char	*find_path_in_env(char **envp)
{
	while (ft_strncmp("PATH", *envp, 4))
		envp++;
	return (*envp + 5);
}

/**
 * @brief ici je recupere le fd du fichier d'entré(le premier ficher)
 * 
 * @param file_name 
 * @param pipex 
 */

void	init_infile(char **file_name, t_ppxb *pipex)
{
	if (!ft_strncmp("here_doc", file_name[1], 9))
		here_doc(file_name[2], pipex);
	else
	{
		pipex->infile = open(file_name[1], O_RDONLY);
		if (pipex->infile < 0)
			msg_error("Infile");
	}
}

/**
 * @brief ici je recupere le fd du fichier de sortie(le dernier ficher)
 * 
 * @param file_name c'est le nom du ficher
 * @param pipex 
 */

void	init_outfile(char *file_name, t_ppxb *pipex)
{
	if (pipex->here_doc)
		pipex->outfile = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0000644);
	else
		pipex->outfile = open(file_name, O_CREAT | O_RDWR | O_TRUNC, 0000644);
	if (pipex->outfile < 0)
		msg_error("Outfile");
}
