/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 15:30:25 by alukongo          #+#    #+#             */
/*   Updated: 2021/11/26 16:12:58 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<stdio.h>
#include"libft.h"
//good

char	*ft_strstr(const char *s1, const char *s2)
{
	int	i;
	int	j;
	int	tmp;

	i = 0;
	if (s2[0] == 0)
		return ((char *)s1);
	while (s1[i])
	{
		j = 0;
		tmp = i;
		while (s1[i + j] && s1[i + j] == s2[j])
			j++;
		if (s2[j] == '\0')
		{
			return ((char *)s1 + i);
		}
		i++;
	}
	return (0);
}
