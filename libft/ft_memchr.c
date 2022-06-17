/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 12:12:26 by alukongo          #+#    #+#             */
/*   Updated: 2021/11/26 12:19:46 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"
//good
void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*ptr_s;
	unsigned char	letter;

	letter = (unsigned char) c;
	ptr_s = (unsigned char *)s;
	while (n)
	{
		if (*ptr_s == letter)
			return (ptr_s);
		n--;
		ptr_s++;
	}
	return (0);
}
