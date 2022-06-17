/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 12:35:06 by alukongo          #+#    #+#             */
/*   Updated: 2021/11/26 13:11:18 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include"libft.h"

//good
char	*ft_strdup(const char *src)
{
	int		i;
	char	*s;

	i = 0;
	s = malloc(sizeof(char) * ft_strlen(src) + 1);
	if (!(s))
		return (0);
	while (src[i])
	{
		s[i] = src[i];
		i++;
	}
	s[i] = '\0';
	return (s);
}
