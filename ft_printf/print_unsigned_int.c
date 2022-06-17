/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_unsigned_int.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/23 14:43:15 by alukongo          #+#    #+#             */
/*   Updated: 2021/12/30 17:14:22 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"ft_printf.h"

int	nb_digit(unsigned int nb, int base)
{
	int	i;

	i = 0;
	if (nb == 0)
		i++;
	while (nb > 0)
	{
		nb = nb / base;
		i++;
	}
	return (i);
}

char	*ft_uitoa(unsigned int nb)
{
	int		i;
	char	*str;

	i = nb_digit(nb, 10);
	if (i == 0)
		i = 1;
	str = malloc(sizeof(char) * i + 1);
	if (!str)
		return (NULL);
	str[i] = '\0';
	while (i > 0)
	{
		str[--i] = (nb % 10) + '0';
		nb = nb / 10;
	}
	return (str);
}

int	ft_unsigned(unsigned int nb)
{
	char	*str;
	int		len;

	len = 1;
	if (nb == 0)
		ft_putchar(0 + '0');
	else
	{
		str = ft_uitoa(nb);
		len = ft_putstr(str);
		free(str);
	}
	return (len);
}
