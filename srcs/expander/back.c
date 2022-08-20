/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   back.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 16:44:29 by denissereno       #+#    #+#             */
/*   Updated: 2022/08/19 15:19:45 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	end_quote(char *str, int i, int quote)
{
	while (str[i])
	{
		if (quote && str[i] == '"' && !is_backed(str, i - 1))
			return (i);
		if (!quote && str[i] == '\'' && !is_backed(str, i - 1))
			return (i);
		i++;
	}
	return (-1);
}

int	backslash_2(t_vector2D *it, char *str, char *result, t_vector2D quoted)
{
	if (it->y > 0 && str[it->y + 1] == 0 && str[it->y - 1] != '\\')
	{
		printf("backslash error\n");
		return (0);
	}
	else if (str[it->y + 1] != '$')
	{
		result[it->x] = str[it->y];
		it->x += 1;
		it->y += 1;
	}
	else if (quoted.x && (str[it->y + 1] == '$'
			|| str[it->y + 1] == '\\'))
	{
		result[it->x] = str[it->y];
		it->x += 1;
		it->y += 1;
	}
	else
		it->y += 1;
	return (1);
}

char	*backslash(char *str, t_global *g)
{
	char		*result;
	t_vector2D	it;
	t_vector2D	quoted;

	it = (t_vector2D){0, 0};
	quoted = (t_vector2D){0, 0};
	result = ft_malloc(sizeof(char) * (ft_strlen(str) * 10 + 1), &g->alloc);
	result[0] = 0;
	while (str[it.y])
	{
		if (str[it.y] == '"' && !quoted.x && !is_backed(str, it.y - 1))
			quoted.y = (quoted.y + 1) % 2;
		if (str[it.y] == '\'' && !quoted.y)
			quoted.x = (quoted.x + 1) % 2;
		if (str[it.y] == '\\')
		{
			if (!backslash_2(&it, str, result, quoted))
				return (NULL);
		}
		else
			result[it.x++] = str[it.y++];
	}
	result[it.x] = 0;
	return (result);
}
