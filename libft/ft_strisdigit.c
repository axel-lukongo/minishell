/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strisdigit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 17:36:02 by denissereno       #+#    #+#             */
/*   Updated: 2022/07/19 17:55:17 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int ft_strisdigit(char *str)
{
    int i;

    i = 0;
    if (!str)
        return (0);
    while (str[i])
    {
        if (!ft_isdigit(str[i]) && str[i] != 0 && str[i] != '-' && str[i] != '+')
            return (0);
        i++;
    }
    return (1);
}