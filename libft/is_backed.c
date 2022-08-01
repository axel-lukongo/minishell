/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_backed.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 20:41:08 by denissereno       #+#    #+#             */
/*   Updated: 2022/07/19 14:45:13 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "libft.h"
# include <stdio.h>

int	is_backed(char *str, int i)
{
	int	backed;

	backed = 0;
	while (i >= 0)
	{
		if (str[i] == '\\')
			backed++;
		else
			break ;
		i--;
		if (i == -1)
			break ;
	}
	return (backed % 2);
}
