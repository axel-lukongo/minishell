/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 12:37:22 by alukongo          #+#    #+#             */
/*   Updated: 2021/11/30 18:52:58 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include<stdio.h>
#include"libft.h"
//good
//je n'avais pas rajouet +1 a mon malloc

static char	*ft_strcpy(char *dest, const char *src)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

static char	*ft_strcat(char *dest, const char *src)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (dest[i] != '\0')
		i++;
	while (src[j] != '\0')
	{
		dest[i + j] = src[j];
		j++;
	}
	dest[i + j] = '\0';
	return (dest);
}

char	*ft_strjoin(char const *s1, char const*s2)
{
	char	*str;
	int		i;

	i = ft_strlen(s1) + ft_strlen(s2);
	if (s1 && s2)
	{
		str = malloc(sizeof(char) * i + 1);
		if (!str)
			return (0);
		ft_strcpy(str, s1);
		ft_strcat(str, s2);
		return (str);
	}
	else
		return (0);
}
