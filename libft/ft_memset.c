/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 12:18:19 by alukongo          #+#    #+#             */
/*   Updated: 2021/11/26 16:42:16 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"libft.h"

void	*ft_memset( void *pointer, int value, size_t count )
{
	unsigned int	i;
	char			*ptr;

	ptr = pointer;
	i = 0;
	while (i < count)
	{
		ptr[i] = (unsigned char)value;
		i++;
	}
	return (ptr);
}
