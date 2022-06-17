/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 15:30:40 by alukongo          #+#    #+#             */
/*   Updated: 2021/11/26 18:10:06 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"
//good

static int	is_charset(const char *set, char c)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*cpy;
	size_t	i;
	size_t	size;

	i = 0;
	if (!s1 || !set)
		return (0);
	while (is_charset(set, *s1))
		s1++;
	size = ft_strlen(s1);
	while (size && is_charset(set, s1[--size]))
		;
	cpy = malloc(sizeof(char) + (size + 1));
	if (!cpy)
		return (0);
	while (i <= size)
	{
		cpy[i] = s1[i];
		i++;
	}
	cpy[i] = '\0';
	return (cpy);
}
