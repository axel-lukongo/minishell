/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/07 09:52:24 by alukongo          #+#    #+#             */
/*   Updated: 2021/11/26 16:00:49 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<unistd.h>
#include"libft.h"
//good
void	ft_putnbr(int nb)
{
	unsigned int	nbr;

	if (nb < 0)
	{
		ft_putchar('-');
		nb = nb * (-1);
	}
	nbr = (unsigned int)nb;
	if (nbr <= 9)
		ft_putchar(nbr + '0');
	if (nbr >= 10)
	{
		ft_putnbr (nbr / 10);
		ft_putchar(nbr % 10 + '0');
	}
}
