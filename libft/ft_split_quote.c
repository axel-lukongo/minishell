/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_quote.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 17:47:05 by darian            #+#    #+#             */
/*   Updated: 2022/07/19 14:27:05 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static int	count_words_quote(char const *s, char c)
{
	int	i;
	int	w;
	int	d_quoted;
	int	s_quoted;

	i = 0;
	w = 0;
	s_quoted = 0;
	d_quoted = 0;
	while (s[i])
	{
		if (s[i] == '"'  && !is_backed((char *)s, i - 1) && !s_quoted)
			d_quoted = (d_quoted + 1) % 2;
		if (s[i] == '\'' && !d_quoted)
			s_quoted = (s_quoted + 1) % 2;
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == 0) && (!s_quoted && !d_quoted))
			w++;
		i++;
	}
	return (w);
}

typedef struct	s_vector2D
{
	int	x;
	int	y;
}	t_vector2D;

static char	*ft_strncpy_split_quote(char const *src, size_t n, t_alloc *alloc)
{
	size_t	i;
	size_t	j;
	char	*dest;
	t_vector2D	quote;
	t_vector2D	last_q;

	i = 0;
	j = 0;
	quote = (t_vector2D){0, 0};
	last_q = (t_vector2D){0, 0};
	dest = ft_malloc(sizeof(char) * n, &alloc);
	if (!dest)
		return (NULL);
	while (i < n - 1 && src[i] != '\0')
	{
		if (src[i] == '"' && !is_backed((char *)src, i - 1) && !quote.x)
		{
			quote.y = (quote.y + 1) % 2;
			last_q.y = 0;
			if (quote.y == 0)
				last_q.y = 1;
		}
		else if (src[i] == '\'' && !quote.y)
		{
			quote.x = (quote.x + 1) % 2;
			last_q.x = 0;
			if (quote.x == 0)
				last_q.x = 1;
		}
		else if (src[i] == '\\')
		{
			if ((quote.x || quote.y) && src[i + 1] != '\\')
				dest[j++] = src[i + 1];
			else
			{
				dest[j++] = src[i + 1];
				i+= 1;
			}
		}
		else if (src[i] == '"' && is_backed((char *)src, i - 1))
			dest[j++] = src[i];
		else if ((!(src[i] == '"' && (quote.y || last_q.y)) && !(src[i] == '\'' && (quote.x || last_q.x))) || (src[i] == '"' && quote.x) || (src[i] == '\'' && quote.y))
			dest[j++] = src[i];
		i++;
	}
	dest[j] = '\0';
	return (dest);
}

static char	**ft_split_body_quote(char const *s, char c, char **split, t_alloc *alloc)
{
	int	j;
	int	k;
	int	i;
	int	d_quoted;
	int	s_quoted;

	i = 0;
	d_quoted = 0;
	s_quoted = 0;
	j = 0;
	k = 0;
	while (s[i])
	{
		while (s[i + j])
		{
			if (s[i + j] == '"' && !is_backed((char *)s, i + j - 1) && !s_quoted)
				d_quoted = (d_quoted + 1) % 2;
			if (s[i + j] == '\'' && !d_quoted)
				s_quoted = (s_quoted + 1) % 2;
			if (s[i + j] == c && (!s_quoted && !d_quoted))
				break ;
			j++;
		}
		if (j != 0)
		{
			split[k] = ft_strncpy_split_quote(s + i, j + 1, alloc);
			if (!split[k++])
				return (NULL);
			i += j - 1;
			j = 0;
		}
		i++;
	}
	split[k] = 0;
	return (split);
}

char	**ft_split_quote(char const *s, char c, t_alloc *alloc)
{
	char	**split;

	if (s == NULL)
		return (NULL);
	split = ft_malloc(sizeof(char *) * (count_words_quote(s, c) + 1), &alloc);
	if (!split)
		return (NULL);
	return (ft_split_body_quote(s, c, split, alloc));
}
