/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_64.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 18:04:36 by denissereno       #+#    #+#             */
/*   Updated: 2022/07/19 18:16:13 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdint.h>

u_int64_t ft_atoi_u64(char *nptr)
{
	int		n;
	u_int64_t result;

	n = 0;
	result = 0;
	while (nptr[n] == ' ' || nptr[n] == '\f' || nptr[n] == '\n'
		|| nptr[n] == '\r' || nptr[n] == '\t' || nptr[n] == '\v')
		n++;
	while ((nptr[n] >= '0' && nptr[n] <= '9'))
	{
		result = result * 10 + nptr[n] - 48;
		n++;
	}
	return (result);
}