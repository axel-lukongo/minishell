/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 12:16:12 by alukongo          #+#    #+#             */
/*   Updated: 2021/11/30 18:33:06 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"
//good
static char	*ft_strnew(size_t size)
{
	char	*str;

	str = (char *)malloc(sizeof(char *) * size + 1);
	if (!str)
	{
		return (NULL);
	}
	ft_bzero(str, size);
	return (str);
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char	*tmp;

	tmp = ft_strnew(n);
	if (src == dest)
		return (dest);
	if (tmp)
	{
		tmp = ft_memcpy(tmp, src, n);
		dest = ft_memcpy(dest, tmp, n);
	}
	free(tmp);
	return (dest);
}
