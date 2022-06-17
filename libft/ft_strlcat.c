/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 13:08:21 by alukongo          #+#    #+#             */
/*   Updated: 2021/11/26 13:22:15 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"
#include<stdio.h>
//good
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	size_src;
	size_t	size_dest;

	i = 0;
	size_dest = 0;
	size_src = ft_strlen(src);
	while (dst[size_dest] != '\0' && size_dest < dstsize)
		size_dest++;
	if (dstsize == 0)
		return (size_src);
	if (size_dest == dstsize)
		return (size_src + dstsize);
	while (src[i] && (size_dest + i) < dstsize - 1)
	{
		dst[size_dest + i] = src[i];
		i++;
	}
	dst[size_dest + i] = '\0';
	return (size_dest + size_src);
}
