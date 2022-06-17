/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 11:45:29 by alukongo          #+#    #+#             */
/*   Updated: 2021/11/29 15:48:37 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"
//good
//je negerais pas le cas où n = 0 
int	size_nbr(int n)
{
	unsigned int	size;

	size = 0;
	if (n <= 0)
		size = 1;
	while (n > 0 || n < 0)
	{
		size++;
		n = n / 10;
	}
	return (size);
}

char	*ft_itoa(int n)
{
	int		sign;
	char	*str;
	int		size;

	sign = n;
	size = size_nbr(n);
	str = malloc(sizeof(char) * (size + 1));
	if (!str)
		return (0);
	str[size] = '\0';
	while (size)
	{
		if (n >= 0)
			str[--size] = n % 10 + 48;
		else
			str[--size] = -1 * (n % 10) + 48;
		n /= 10;
	}
	if (sign < 0)
		str[size] = '-';
	return (str);
}
