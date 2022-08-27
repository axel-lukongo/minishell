/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 13:40:57 by alukongo          #+#    #+#             */
/*   Updated: 2022/08/26 18:29:56 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief ici je verifie si je dois faire un here_doc
 * 
 * @param arg 
 * @param pipex 
 * @return int 
 */

int	args_in(char *arg, t_ppxb *pipex)
{
	if (arg && !ft_strncmp("here_doc", arg, 9))
	{
		pipex->here_doc = 1;
		return (6);
	}
	else
	{
		pipex->here_doc = 0;
		return (5);
	}
}

/**
 * @brief je rentre dans cette fonction si je dois faire un here_doc,
 * je vais afficher "heredoc>" puis avec mon gnl je vais lire
 * tous ce que je vais ecrire dans mon shell et l'ecrire dans un
 * fichier. si je tombe sur le mots clef je break
 * 
 * tu pourras essayer de remplacer mon gnl par readline askip.
 * 
 * @param argv 
 * @param pipex 
 */

void	here_doc(char *argv, t_ppxb *pipex)
{
	int		file;
	char	*buf;

	file = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0000644);
	if (file < 0)
		msg_error("here_doc");
	while (1)
	{
		write(1, "heredoc> ", 9);
		if (get_next_line(0, &buf) < 0)
			exit(1);
		if (!ft_strncmp(argv, buf, ft_strlen(argv) + 1))
			break ;
		write(file, buf, ft_strlen(buf));
		write(file, "\n", 1);
		free(buf);
	}
	free(buf);
	close(file);
	pipex->infile = open(".heredoc_tmp", O_RDONLY);
	if (pipex->infile < 0)
	{
		unlink(".heredoc_tmp");
		msg_error("here_doc");
	}
}
