/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/22 15:35:54 by alukongo          #+#    #+#             */
/*   Updated: 2022/06/18 00:24:15 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"ft_printf.h"

int	ft_putchar(char c)
{
	write(1, &c, 1);
	return (1);
}

int	ft_strlen_p(char *s)
{
	int	i;

	i = 0;
	if (s)
	{
		while (s[i])
			i++;
	}
	return (i);
}

int	ft_putstr(char *str)
{
	int	i;

	i = 0;
	if (str)
	{
		while (str[i] != '\0')
			ft_putchar(str[i++]);
	}
	else
	{
		write(1, "(null)", 6);
		i = 6;
	}
	return (i);
}

int	ft_putnbr(int nbr)
{
	int		lb;
	char	*str;

	lb = 0;
	if (nbr < 0)
	{
		lb += ft_putchar('-');
		nbr *= -1;
	}
	str = ft_uitoa(nbr);
	lb += ft_putstr(str);
	free(str);
	return (lb);
}
