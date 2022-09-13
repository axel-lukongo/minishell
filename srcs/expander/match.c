/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 14:40:06 by darian            #+#    #+#             */
/*   Updated: 2022/09/13 12:46:54 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**exec_ls(t_alloc **alloc, t_global *g)
{
	int		fd[2];
	pid_t	pid;
	char	foo[4096];
	int		read_val;

	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		dup2 (fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		execve("/bin/ls", (char *[2]){"ls", NULL}, g->char_env);
	}
	else
	{
		close(fd[1]);
		read_val = read(fd[0], foo, sizeof(foo));
		foo[read_val] = 0;
		return (ft_split(foo, '\n', alloc));
		wait(NULL);
	}
	return (NULL);
}

int	match(char *wildcard, char *to_match)
{
	if (*wildcard == 0 && *to_match == 0)
		return (1);
	if (*wildcard == '*' && *(wildcard + 1) != 0 && *to_match == 0)
		return (0);
	if (*wildcard == *to_match)
		return (match(wildcard + 1, to_match + 1));
	if (*wildcard == '*')
		return (match(wildcard + 1, to_match) || match(wildcard, to_match + 1));
	return (0);
}

int	matching(char **res, char *strs[2], t_alloc **alloc, char **files)
{
	int	j;
	int	matched;

	matched = 0;
	j = 0;
	while (files[j])
	{
		if (match(strs[0], files[j]))
		{
			if (strs[1] || files[j + 1])
				*res = ft_strjoin(*res, ft_strjoin(files[j], " ", alloc),
						alloc);
			else
				*res = ft_strjoin(*res, files[j], alloc);
			matched = 1;
		}
		j++;
	}
	if (!matched)
		return (0);
	return (1);
}

void	match_loop(char *strs[2], char **res, t_alloc **alloc, char **files)
{
	if (is_char(strs[0], '*') > 0)
	{
		if (!matching(res, strs, alloc, files))
		{
			if (strs[1])
				*res = ft_strjoin(*res, ft_strjoin(strs[0], " ", alloc), alloc);
			else
				*res = ft_strjoin(*res, strs[0], alloc);
		}
	}
	else if (strs[0])
	{
		*res = ft_strjoin(*res, strs[0], alloc);
		if (strs[1] != NULL)
			*res = ft_strjoin(*res, " ", alloc);
	}
}

char	*wildcard(char *str, t_alloc *alloc, t_global *g)
{
	char		**files;
	char		**strs;
	char		*res;
	int			i;

	files = exec_ls(&alloc, g);
	strs = ft_split_quote(str, ' ', alloc);
	res = NULL;
	i = 0;
	while (strs[i])
	{
		match_loop((char *[2]){strs[i], strs[i + 1]}, &res, &alloc, files);
		i++;
	}
	return (res);
}

// WILDCARD CASE :
// sr* -> Comparer tout les dossier/fichier commençant par 'sr' et les ajouter 
// pour le displaying
// * -> tout les fichier/dossier courant
// *.c -> comparer la fin des noms de fichier/dossier en commencant par .c et 
// finissant par \0
// sr*.c -> comparer tout les debut de fichier/dossier les ajpoute dans une 
// liste puis comparer les fins de tous par '.c' jusqua \0 et ajouter a la 
// liste finale.
// RECURSIVEMENT
// tout les cas d'avant + / a a la fin donc :
// sr*/... -> sr*/sr* || sr*/* || sr*/*.c || sr*/sr*.c
// */... -> */sr* || */* || */*.c || */sr*.c
// *.c/.. .....
// sr*.c/.. .....
// Checker tout avant le / avec les 4 paterne possible puis recursive checker 
// chaque dossier valide puis continuer recursivement jusqua la fin des /