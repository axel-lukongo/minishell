/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 18:32:29 by alukongo          #+#    #+#             */
/*   Updated: 2022/09/11 18:40:52 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*del_start_end_char(char *str, char c, t_alloc **alloc)
{
	int		i;
	int		n;
	int		pos;
	char	*new;

	i = 0;
	n = 0;
	while (str[n] == c)
		n++;
	i = ft_strlen(str) - 1;
	while (i >= 0 && str[i] == c)
	{
		i--;
		n++;
	}
	pos = i;
	new = ft_malloc((sizeof(char) * ft_strlen(str) - n + 1), alloc);
	n = 0;
	i = 0;
	while (str[n] == c)
		n++;
	while (n <= pos)
		new[i++] = str[n++];
	new[i] = 0;
	return (new);
}

void print_err(t_global *g, char **s, int i)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(s[i], 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	g->last_return = 1;
}

char	**split_two(char **new, char *str, char c, t_alloc **alloc)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i] != c && str[i])
		i++;
	new = ft_malloc(sizeof(char *) * 3, alloc);
	new[0] = ft_malloc(sizeof(char) * (i + 1), alloc);
	new[1] = ft_malloc(sizeof(char) * (ft_strlen(str) - i + 1), alloc);
	i = 0;
	while (str[i] != c && str[i])
		new[0][j++] = str[i++];
	new[0][j] = 0;
	j = 0;
	i++;
	while (str[i])
		new[1][j++] = str[i++];
	new[1][j] = 0;
	new[2] = NULL;
	return (new);
}

char	**ft_split_first(char *str, char c, t_alloc **alloc)
{
	char	**new;
	int		i;

	i= 0;
	new = NULL;
	while (str[i] != c && str[i])
		i++;
	if (!str[i])
	{
		new = ft_malloc(sizeof(char *) * 2, alloc);
		new[0] = ft_strdup(str, alloc);
		new[1] = NULL;
		return (new);
	}
	else
		return(split_two(new, str, c, alloc));
}

int	is_valid_identifier(char *str)
{
	int	i;

	i = 0;
	if (ft_strlen(str) == 0)
		return (0);
	if (str[0] >= '0' && str[0] <= '9')
		return (0);
	while (str[i])
	{
		if ((str[i] < 'A' || str[i] > 'Z') && (str[i] < 'a' || str[i] > 'z')
			&& (str[i] < '0' || str[i] > '9') && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
