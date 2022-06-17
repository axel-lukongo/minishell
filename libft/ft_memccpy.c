/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 12:11:24 by alukongo          #+#    #+#             */
/*   Updated: 2021/11/30 14:51:00 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"
//good

void	*ft_memccpy(void *dest, const void *src, int c, size_t n)
{
	unsigned char	*ptr_src;
	unsigned char	*ptr_dest;

	ptr_src = (unsigned char *)src;
	ptr_dest = (unsigned char *)dest;
	while (n)
	{
		*ptr_dest = *ptr_src;
		if (*ptr_dest == (unsigned char) c)
		{
			return (ptr_dest + 1);
		}
		ptr_dest++;
		ptr_src++;
		n--;
	}
	return (0);
}
/*
void	*ft_memccpy(void *dest, const void *src, int c, size_t n)
{
	unsigned char	*p_dest;
	unsigned char	*p_src;

	p_dest = (unsigned char *)dest;
	p_src = (unsigned char *)src;
	while (n)
	{
		*p_dest = *p_src;
		if (*p_src == (unsigned char)c)
			return (p_dest + 1);
		p_dest++;
		p_src++;
		n--;
	}
	return (0);
}*/
