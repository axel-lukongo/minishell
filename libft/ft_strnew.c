/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 13:18:10 by alukongo          #+#    #+#             */
/*   Updated: 2021/11/29 18:34:08 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"
//good
char	*ft_strnew(size_t size)
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
