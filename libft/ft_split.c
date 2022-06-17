/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 13:26:44 by alukongo          #+#    #+#             */
/*   Updated: 2021/11/30 17:46:56 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<stdlib.h>
#include<stdio.h>
//je ne gerais pas bien le cas ou il y'avais plusieur set a la fin 
//donc j'avais une ligne en trop
//je ne gerais pas bien le cas ou le charset etais un '\0'
static int	length_word(const char *s, char c)
{
	int	len;

	len = 0;
	while (s[len] && s[len] != c)
	{
		len++;
	}
	return (len);
}

static char	**free_alloc(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return (NULL);
}

static int	nb_word(const char *s, char c)
{
	int	word;
	int	i;

	i = 0;
	word = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] != c && s[i])
		{
			word++;
			while (s[i] && s[i] != c)
				i++;
		}
	}
	return (word);
}

static char	**writing(char **tab, const char *s, char c, int word)
{
	int	i;
	int	j;

	j = 0;
	i = -1;
	while (++i < word)
	{
		while (*s == c)
			s++;
		tab[i] = malloc(sizeof(char) * (length_word(s, c) + 1));
		if (!tab[i])
			free_alloc(tab);
		j = 0;
		while (*s && *s != c)
		{
			tab[i][j++] = *s;
			s++;
		}
		tab[i][j] = '\0';
	}
	tab[i] = 0;
	return (tab);
}
//good

char	**ft_split(char const *s, char c)
{
	int			word;
	char		**tab;

	if (!s)
		return (0);
	word = nb_word(s, c);
	while (*s == c)
		s++;
	tab = malloc(sizeof (char *) * (word + 1));
	if (!tab)
		free_alloc(tab);
	tab = writing(tab, s, c, word);
	return (tab);
}
