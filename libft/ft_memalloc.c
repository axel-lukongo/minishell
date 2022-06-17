/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 12:08:31 by alukongo          #+#    #+#             */
/*   Updated: 2021/11/26 12:18:16 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"
//good
void	*ft_memalloc(size_t size)
{
	void	*str;

	str = (void *)malloc(sizeof (str) * size);
	if (!str)
		return (NULL);
	ft_bzero(str, size);
	return (str);
}
