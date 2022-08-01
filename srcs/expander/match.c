/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 14:40:06 by darian            #+#    #+#             */
/*   Updated: 2022/07/18 16:29:07 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_char(char *str, char c)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	when_is_char(char *str, char c)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (0);
}

int	only_wildcard(char **files, t_alloc **alloc, char **res, int is_after)
{
	int	j;
	int	n;

	n = 0;
	j = 0;
	while (files[j])
	{
		*res = ft_strjoin(*res, files[j], alloc);
		if (files[j + 1] != NULL)
		{
			*res = ft_strjoin(*res, " ", alloc);
			n++;
		}
		j++;
	}
	if (n == 0)
		return (0);
	if (is_after)
		*res = ft_strjoin(*res, " ", alloc);
	return (1);
}

int	match_start(char *str, char *to_match)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (to_match[i])
	{
		if (str[j] != to_match[i])
			return (0);
		j++;
		i++;
	}
	return (1);
}

int	match_end(char *str, char *to_match)
{
	int	i;
	int	j;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
	{
		j = 0;
		while (to_match[j] == str[i] && str[i] && to_match[j])
		{
			i++;
			j++;
		}
		if (to_match[j] == 0 && str[i] == 0)
			return (1);
		if (!str[i])
			break ;
		i++;
	}
	return (0);
}

int	is_matching_between(char *str, char *start, char *end)
{
	int	i;
	int	j;
	
	i = 0;
	j = 0;
	while (start[i])
	{
		if (str[j] != start[i])
			return (0);
		j++;
		i++;
	}
	i = 0;
	while (end[i] && str[j])
	{
		i = 0;
		while (end[i] == str[j] && str[j] && end[i])
		{
			i++;
			j++;
		}
		if (end[i] == 0 && str[j] == 0)
			return (1);
		if (!str[j])
			break ;
		j++;
	}
	return (0);
}

int	wildcard_between(char **files, char **res, t_alloc **alloc, char *str, int is_after)
{
	int		i;
	int		n;
	char	*to_match_start;
	char	*to_match_end;

	i = 0;
	n = 0;
	to_match_start = ft_substr(str, 0, when_is_char(str, '*'), *alloc);
	to_match_end = ft_substr(str, when_is_char(str, '*') + 1, ft_strlen(str),*alloc);
	while (files[i])
	{
		if (is_matching_between(files[i], to_match_start, to_match_end))
		{
			if (n != 0)
				*res = ft_strjoin(*res, " ", alloc);
			printf("-> %s\n", files[i]);
			*res = ft_strjoin(*res, files[i], alloc);
			n++;
		}
		i++;
	}
	if (n == 0)
		return (0);
	if (is_after)
		*res = ft_strjoin(*res, " ", alloc);
	return (1);
}

int	wildcard_start(char **files, char **res, t_alloc **alloc, char *str, int is_after)
{
	char	*to_match_end;
	int		i;
	int		n;

	n = 0;
	i = 0;
	to_match_end = ft_substr(str, when_is_char(str, '*') + 1, ft_strlen(str),*alloc);
	while (files[i])
	{
		if (match_end(files[i], to_match_end))
		{
			if (n != 0)
				*res = ft_strjoin(*res, " ", alloc);
			*res = ft_strjoin(*res, files[i], alloc);
			n++;
		}
		i++;
	}
	if (n == 0)
		return (0);
	if (is_after)
		*res = ft_strjoin(*res, " ", alloc);
	return (1);
}

int	wildcard_end(char **files, char **res, t_alloc **alloc, char *str, int is_after)
{
	char	*to_match_start;
	int		i;
	int		n;

	n = 0;
	i = 0;
	to_match_start = ft_substr(str, 0, when_is_char(str, '*'), *alloc);
	while (files[i])
	{
		if (match_start(files[i], to_match_start))
		{
			if (n != 0)
				*res = ft_strjoin(*res, " ", alloc);
			*res = ft_strjoin(*res, files[i], alloc);
			n++;
		}
		i++;
	}
	if (n == 0)
		return (0);
	if (is_after)
		*res = ft_strjoin(*res, " ", alloc);
	return (1);
}

char	**exec_ls(t_alloc **alloc)
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
		execl("/bin/ls", "ls", (char *)0);
	}
	else
	{
		close(fd[1]);
		read_val = read(fd[0], foo, sizeof(foo));
		foo[read_val] = 0;
		return (ft_split(foo, '\n', *alloc));
		wait(NULL);
	}
	return (NULL);
}

char	*wildcard(char *str, t_alloc *alloc)
{
	char		**files;
	char		**strs;
	char		*res;
	int			i;
	t_vector2D	no_match;

	files = exec_ls(&alloc);
	strs = ft_split(str, ' ', alloc);
	res = NULL;
	no_match = (t_vector2D){0, 0};
	i = 0; // CHECK BEFORE BACKSLASH TO ADD TO MATCH : \ \ * -> match'  *'
	while (strs[i])
	{
		if (is_char(strs[i], '*') > 0)
		{
			strs[i] = delete_quote(strs[i], &alloc);
			if (strs[i][0] == '*' && strs[i][1] == 0)
			{
				if (!(only_wildcard(files, &alloc, &res, strs[i + 1] != NULL)))
				{
					if (strs[i + 1] != NULL)
						res = ft_strjoin(res, ft_strjoin("*", " ", &alloc) , &alloc);
					else
						res = ft_strjoin(res, "*", &alloc);
				}
			}
			else if (strs[i][0] != '*' && strs[i][ft_strlen(strs[i]) - 1] != '*')
			{
				if (!(wildcard_between(files, &res, &alloc, strs[i], strs[i + 1] != NULL)))
				{
					if (strs[i + 1] != NULL)
						res = ft_strjoin(res, ft_strjoin(strs[i], " ", &alloc), &alloc);
					else
						res = ft_strjoin(res, strs[i], &alloc);
				}
			}
			else if (strs[i][0] == '*' && strs[i][ft_strlen(strs[i]) - 1] != '*')
			{
				if (!(wildcard_start(files, &res, &alloc, strs[i], strs[i + 1] != NULL)))
				{
					if (strs[i + 1] != NULL)
						res = ft_strjoin(res, ft_strjoin(strs[i], " ", &alloc), &alloc);
					else
						res = ft_strjoin(res, strs[i], &alloc);
				}
			}
			else
			{
				if (!(wildcard_end(files, &res, &alloc, strs[i], strs[i + 1] != NULL)))
				{
					if (strs[i + 1] != NULL)
						res = ft_strjoin(res, ft_strjoin(strs[i], " ", &alloc), &alloc);
					else
						res = ft_strjoin(res, strs[i], &alloc);
				}
			}
		}
		else if (strs[i])
		{
			res = ft_strjoin(res, strs[i], &alloc);
			if (strs[i + 1] != NULL)
				res = ft_strjoin(res, " ", &alloc);
		} 
		i++;
	}
	(void)no_match;
	// if (no_match.x)
	// 	res = ft_strjoin(res, delete_quote(str, &alloc), &alloc);
	return (res);
}


// WILDCARD CASE :
// sr* -> Comparer tout les dossier/fichier commençant par 'sr' et les ajouter pour le displaying
// * -> tout les fichier/dossier courant
// *.c -> comparer la fin des noms de fichier/dossier en commencant par .c et finissant par \0
// sr*.c -> comparer tout les debut de fichier/dossier les ajpoute dans une liste puis comparer les fins de tous par '.c' jusqua \0 et ajouter a la liste finale.
// RECURSIVEMENT
// tout les cas d'avant + / a a la fin donc :
// sr*/... -> sr*/sr* || sr*/* || sr*/*.c || sr*/sr*.c
// */... -> */sr* || */* || */*.c || */sr*.c
// *.c/.. .....
// sr*.c/.. .....
// Checker tout avant le / avec les 4 paterne possible puis recursive checker chaque dossier valide puis continuer recursivement jusqua la fin des /