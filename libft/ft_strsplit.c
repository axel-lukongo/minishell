/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 13:26:44 by alukongo          #+#    #+#             */
/*   Updated: 2021/11/25 14:53:39 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<stdlib.h>
#include<stdio.h>

int	length_word(char *s, char c)
{
	int	len;

	len = 0;
	while (s[len] && s[len] != c)
	{
		len++;
	}
	return (len);
}

int	nb_word(char *s, char c)
{
	int	word;
	int	i;

	i = 0;
	word = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] != c)
		{
			word++;
			while (s[i] && s[i] != c)
				i++;
		}
	}
	return (word);
}

char	**writing(char **tab, char *s, char c, int word)
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
			return (0);
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

char	**ft_strsplit(char *s, char c)
{
	int			word;
	char		**tab;

	word = nb_word(s, c);
	tab = malloc(sizeof (char *) * (word + 1));
	if (!s || !tab)
		return (0);
	tab = writing(tab, s, c, word);
	return (tab);
}
