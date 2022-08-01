/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 17:42:28 by darian            #+#    #+#             */
/*   Updated: 2022/07/31 10:29:34 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		count_nb(int nb)
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
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_')
		return (1);
	return (0);
}

void	double_dollar(char **result, t_global *g, int *i, int *j, char *str)
{
	if (is_backed(str, *i - 1))
	{
		*result = ft_strjoin(*result, "$$", &g->alloc);
		*i += 2;
		*j = ft_strlen(*result);
		return ;
	}
	*result = ft_strjoin(*result, ft_itoa(g->sh_pid, g->alloc), &g->alloc);
	*i += 2;
	*j = ft_strlen(*result);
}

void	dollar_question(char **result, t_global *g, int *i, int *j, char *str)
{
	if (is_backed(str, *i - 1))
	{
		*result = ft_strjoin(*result, "$?", &g->alloc);
		*i += 2;
		*j = ft_strlen(*result);
		return ;
	}
	*result = ft_strjoin(*result, ft_itoa(g->last_return, g->alloc), &g->alloc);
	*i += 2;
	*j = ft_strlen(*result);
}

void	dollar_number(char **result, t_global *g, int *i, int *j, char *str)
{
	char	*nb;

	if (is_backed(str, *i - 1))
	{
		nb = ft_malloc(sizeof(char) * 3, &g->alloc);
		nb[0] = str[*i];
		nb[1] = str[*i + 1];
		nb[2] = 0;
		*result = ft_strjoin(*result, nb, &g->alloc);
		*i += 2;
		*j = ft_strlen(*result);
		return ;
	}
	*i += 2;
}

void	dollar_var(char *str, t_vector2D *it, t_global *g, char **result)
{
	char	*var;
	char	*tmp;

	var = get_var_name(str, it->y);
	if (!var)
		*result = ft_strjoin(*result, " ", &g->alloc);
	else
	{
		tmp = get_value_by_name(g->env, var);
		if (!tmp && !is_backed(str, it->y - 1))
			it->y += ft_strlen(var) + 1;
		else if (is_backed(str, it->y - 1))
		{
			*result = ft_strjoin(*result, ft_strjoin("$", var, &g->alloc), &g->alloc);
			it->y += ft_strlen(var) + 1;
			it->x += ft_strlen(var) + 1;
		}
		else
		{
			*result = ft_strjoin(*result, tmp, &g->alloc);
			it->x = ft_strlen(*result);
			it->y += ft_strlen(var) + 1;
		}
	}
}

char	*extract_env_var_call(char *str, t_global *g)
{
	char		*result;
	t_vector2D	it;
	t_vector2D	quoted;

	it.x = 0;
	it.y = 0;
	quoted = (t_vector2D){0, 0};
	result = ft_malloc(sizeof(char) * (ft_strlen(str) * 10 + 1), &g->alloc);
	result = NULL;
	while(str[it.y])
	{
		if (str[it.y] == '"' && !quoted.x)
			quoted.y = (quoted.y + 1) % 2;
		if (str[it.y] == '\'' && !quoted.y)
			quoted.x = (quoted.x + 1) % 2;
		if (str[it.y] == '$' && str[it.y + 1] == '$' && !quoted.x)
			double_dollar(&result, g, &it.y, &it.x, str);
		else if (str[it.y] == '$' && ((str[it.y + 1] >= '0' && str[it.y + 1] <= '9') || str[it.y + 1] == '*') && !quoted.x)
			dollar_number(&result, g, &it.y, &it.x, str);
		else if (str[it.y] == '$' && str[it.y + 1] == '?' && !quoted.x )
			dollar_question(&result, g, &it.y, &it.x, str);
		else if (str[it.y] == '$' && is_shell_char_var_allowed(str[it.y + 1]) && !quoted.x)
			dollar_var(str, &it, g, &result);
		else
		{
			if (!(str[it.y] == '$' && str[it.y + 1] == '"' && !quoted.y) && !(str[it.y] == '$' && str[it.y + 1] == '\'' && !quoted.x))
				result = ft_strjoin_char(result, str[it.y], &g->alloc);
			it.x += 1;
			it.y += 1;
		}
	}
	return (result);
}

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

char	*backslash(char *str, t_global *g)
{
	char		*result;
	t_vector2D	it;
	t_vector2D	quoted;
	int			end;

	it = (t_vector2D){0, 0};
	quoted = (t_vector2D){0, 0};
	result = ft_malloc(sizeof(char) * (ft_strlen(str) * 10 + 1), &g->alloc);
	result[0] = 0;
	end = -1;
	while (str[it.y])
	{
		if (str[it.y] == '"' && !quoted.x && !is_backed(str, it.y - 1))
		{
			quoted.y = (quoted.y + 1) % 2;
			if (quoted.y)
				end = end_quote(str, it.y + 1, 1);
		}
		if (str[it.y] == '\'' && !quoted.y)
		{
			quoted.x = (quoted.x + 1) % 2;
			if (quoted.x)
				end = end_quote(str, it.y + 1, 0);
		}
		if (str[it.y] == '\\')
		{
			printf("->%c\n", str[it.y]);
			if (it.y > 0 && str[it.y + 1] == 0 && str[it.y - 1] != '\\')
			{
				printf("backslash error\n");
				return (NULL);
			}
			else if (str[it.y + 1] != '$')
				result[it.x++] = str[it.y++];
			else if (quoted.x && (str[it.y + 1] == '$' || str[it.y + 1] == '\\'))
				result[it.x++] = str[it.y++];
			else
				it.y++;
		}
		else/* if (end != it.y && end != -1)*/
		{
			printf("=> %c\n", str[it.y]);
			result[it.x++] = str[it.y++];
		}
		// else
		// 	it.y++;
		if (!str[it.y])
			break ;
	}
	(void)end;
	result[it.x] = 0;
	return (result);
}

void expander(t_tree *ast, t_global *g)
{
	if (ast == NULL)
		return ;
	else
	{
		if (ast->type == ENV || ast->type == WILDENV || ast->type == WILDENVBACK || ast->type == BACKENV) // SI C'EST UNE VAR ENVIRONNEMENT
			ast->value = extract_env_var_call(ast->value, g);
		if (ast->type == WILDCARD || ast->type == WILDENV || ast->type == WILDENVBACK || ast->type == WILDBACK) // SI C'est UN WILDCARD
			ast->value = wildcard(ast->value, g->alloc);
		if (ast->type == BACKSLASH || ast->type == WILDENV || ast->type == BACKENV || ast->type == WILDENVBACK) // SI C'EST UN BACKSLASH
			ast->value = backslash(ast->value, g); 
	}
	if (ast->left)
		expander(ast->left, g);
	if (ast->right)
		expander(ast->right, g);
}

// echo \ * - > zsh: no matches found:  * -> parce que \ . (\ espace) collé a * donc match :' *'
// echo \ \ * -> pareil mais 2 espace...

// CORRIGER LES $VAR AU MILIEU STRING QUI SE CONCAT EN UNE STRING (DU COUP IMPOSSIBLE VOIR VARIABLE DANS PARSING-) echo "HOME"'test'*'lol'$HOME'wow' devient echo 'HOMEtest*lol$HOMEwow'.. CONCATENER $HOME DANS UNE STRING DANS CONCAT_TO_LAST (creer new fonction) 