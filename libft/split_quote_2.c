/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_quote_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 17:26:31 by denissereno       #+#    #+#             */
/*   Updated: 2022/09/12 17:27:54 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_lol_2(char *str[2], t_vector2D *quote, t_vector2D *last_q,
	t_vector2D *it)
{
	if (str[0][it->x] == '\\')
	{
		if ((quote->y && str[0][it->x + 1] != '\\'
			&& str[0][it->x + 1] != '"') || quote->x)
			str[1][it->y++] = str[0][it->x];
		else
			str[1][it->y++] = str[0][it->x++ + 1];
	}
	else if (str[0][it->x] == '"' && is_backed((char *)str[0], it->x - 1))
		str[1][it->y++] = str[0][it->x];
	else if ((!(str[0][it->x] == '"' && (quote->y || last_q->y))
		&& !(str[0][it->x] == '\'' && (quote->x || last_q->x)))
		|| (str[0][it->x] == '"' && quote->x) || (str[0][it->x] == '\''
			&& quote->y))
		str[1][it->y++] = str[0][it->x];
	return (str[1]);
}

char	*ft_lol(char *str[2], t_vector2D *quote, t_vector2D *last_q,
	t_vector2D *it)
{
	if (str[0][it->x] == '"' && !is_backed((char *)str[0], it->x - 1)
		&& !quote->x)
	{
		quote->y = (quote->y + 1) % 2;
		last_q->y = 0;
		if (quote->y == 0)
			last_q->y = 1;
	}
	else if (str[0][it->x] == '\'' && !quote->y)
	{
		quote->x = (quote->x + 1) % 2;
		last_q->x = 0;
		if (quote->x == 0)
			last_q->x = 1;
	}
	else
		return (ft_lol_2(str, quote, last_q, it));
	return (str[1]);
}
