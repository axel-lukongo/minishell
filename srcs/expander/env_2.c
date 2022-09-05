/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/18 18:25:59 by denissereno       #+#    #+#             */
/*   Updated: 2022/09/05 15:10:42 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_nb(int nb)
{
	int	i;

	i = 0;
	while (nb)
	{
		nb /= 10;
		i++;
	}
	return (i);
}

char	*get_var_name(char *str, int i)
{
	char	*res;
	int		len;
	int		k;

	len = 0;
	if (str[i] == '$')
		i++;
	k = i;
	while (is_shell_char_var_allowed(str[i]) && str[i])
	{
		i++;
		len++;
	}
	res = malloc(sizeof(char) * (len + 1));
	len = 0;
	while (is_shell_char_var_allowed(str[k]) && str[k])
	{
		res[len] = str[k];
		len++;
		k++;
	}
	res[len] = 0;
	return (res);
}

int	is_shell_char_var_allowed(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9') || c == '_')
		return (1);
	return (0);
}

void	double_dollar(char **result, t_global *g, t_vector2D *it, char *str)
{
	if (is_backed(str, it->y - 1))
	{
		*result = ft_strjoin(*result, "$$", &g->alloc);
		it->y += 2;
		it->x = ft_strlen(*result);
		return ;
	}
	*result = ft_strjoin(*result, ft_itoa(g->sh_pid, g->alloc), &g->alloc);
	it->y += 2;
	it->x = ft_strlen(*result);
}

void	dollar_question(char **result, t_global *g, t_vector2D *it, char *str)
{
	if (is_backed(str, it->y - 1))
	{
		*result = ft_strjoin(*result, "$?", &g->alloc);
		it->y += 2;
		it->x = ft_strlen(*result);
		return ;
	}
	*result = ft_strjoin(*result, ft_itoa(g->last_return, g->alloc), &g->alloc);
	it->y += 2;
	it->x = ft_strlen(*result);
}
