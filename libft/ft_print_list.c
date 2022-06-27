/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 12:19:53 by alukongo          #+#    #+#             */
/*   Updated: 2022/06/26 17:29:31 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include<stdio.h>
//good
t_list	*print_list(t_list *beta)
{
	char	*c;
	t_list *test;
	
	test = beta;
	while (beta)
	{
		c = (char *)beta->content;
		printf("[%s]--->", c);
		beta = beta->next;
	}
	printf("NULL");
	printf("\n");
	return(test);
}
