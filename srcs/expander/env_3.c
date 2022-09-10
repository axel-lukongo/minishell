/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/10 17:45:17 by denissereno       #+#    #+#             */
/*   Updated: 2022/09/10 17:45:26 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	increment_it(t_vector2D *it, int value1, int value2)
{
	it->x += value1;
	it->y += value2;
}

void	dollar_var(char *str, t_vector2D *it, t_global *g, char **result)
{
	char	*var;
	char	*tmp;

	var = get_var_name(str, it->y, g);
	if (!var)
		*result = ft_strjoin(*result, " ", &g->alloc);
	else
	{
		if (cd_specific_case(str, g))
			g_p->error_cd = 1;
		tmp = get_value_by_name(g->env, var);
		if (!tmp && !is_backed(str, it->y - 1))
			it->y += ft_strlen(var) + 1;
		else if (is_backed(str, it->y - 1))
		{
			*result = ft_strjoin(*result, ft_strjoin("$", var, &g->alloc),
					&g->alloc);
			increment_it(it, ft_strlen(var) + 1, ft_strlen(var) + 1);
		}
		else
		{
			*result = ft_strjoin(*result, tmp, &g->alloc);
			increment_it(it, ft_strlen(var) + 1, ft_strlen(var) + 1);
		}
	}
}
