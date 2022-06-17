/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 11:43:15 by alukongo          #+#    #+#             */
/*   Updated: 2021/11/26 12:10:29 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include"libft.h"
//good
void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, 0, n);
}
