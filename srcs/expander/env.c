/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 17:42:28 by darian            #+#    #+#             */
/*   Updated: 2022/08/18 18:25:54 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	dollar_number(char **result, t_global *g, t_vector2D *it, char *str)
{
	char	*nb;

	if (is_backed(str, it->y - 1))
	{
		nb = ft_malloc(sizeof(char) * 3, &g->alloc);
		nb[0] = str[it->y];
		nb[1] = str[it->y + 1];
		nb[2] = 0;
		*result = ft_strjoin(*result, nb, &g->alloc);
		it->y += 2;
		it->x = ft_strlen(*result);
		return ;
	}
	it->y += 2;
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
			*result = ft_strjoin(*result, ft_strjoin("$", var, &g->alloc),
					&g->alloc);
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

void	loop_env(char *str, t_vector2D *it, char **result, t_global *g)
{
	t_vector2D	quoted;

	quoted = (t_vector2D){0, 0};
	if (str[it->y] == '"' && !quoted.x)
		quoted.y = (quoted.y + 1) % 2;
	if (str[it->y] == '\'' && !quoted.y)
		quoted.x = (quoted.x + 1) % 2;
	if (str[it->y] == '$' && str[it->y + 1] == '$' && !quoted.x)
		double_dollar(result, g, it, str);
	else if (str[it->y] == '$' && ((str[it->y + 1] >= '0'
				&& str[it->y + 1] <= '9') || str[it->y + 1] == '*')
		&& !quoted.x)
		dollar_number(result, g, it, str);
	else if (str[it->y] == '$' && str[it->y + 1] == '?' && !quoted.x)
		dollar_question(result, g, it, str);
	else if (str[it->y] == '$' && is_shell_char_var_allowed(str[it->y + 1])
		&& !quoted.x)
		dollar_var(str, it, g, result);
	else
	{
		if (!(str[it->y] == '$' && str[it->y + 1] == '"' && !quoted.y)
			&& !(str[it->y] == '$' && str[it->y + 1] == '\'' && !quoted.x))
			*result = ft_strjoin_char(*result, str[it->y], &g->alloc);
		*it = (t_vector2D){it->x + 1, it->y + 1};
	}
}

char	*extract_env_var_call(char *str, t_global *g)
{
	char		*result;
	t_vector2D	it;

	it.x = 0;
	it.y = 0;
	result = ft_malloc(sizeof(char) * (ft_strlen(str) * 10 + 1), &g->alloc);
	result = NULL;
	while (str[it.y])
		loop_env(str, &it, &result, g);
	return (result);
}

void	expander(t_tree *ast, t_global *g)
{
	if (ast == NULL)
		return ;
	else
	{
		if (ast->type == ENV || ast->type == WILDENV
			|| ast->type == WILDENVBACK || ast->type == BACKENV)
			ast->value = extract_env_var_call(ast->value, g);
		if (ast->type == WILDCARD || ast->type == WILDENV
			|| ast->type == WILDENVBACK || ast->type == WILDBACK)
			ast->value = wildcard(ast->value, g->alloc);
		if (ast->type == BACKSLASH || ast->type == WILDENV
			|| ast->type == BACKENV || ast->type == WILDENVBACK)
			ast->value = backslash(ast->value, g);
	}
	if (ast->left)
		expander(ast->left, g);
	if (ast->right)
		expander(ast->right, g);
}

// echo \ * - > zsh: no matches found:  * -> parce que \ . (\ espace) collé a 
// * donc match :' *'
// echo \ \ * -> pareil mais 2 espace...

// CORRIGER LES $VAR AU MILIEU STRING QUI SE CONCAT EN UNE STRING (DU COUP 
// IMPOSSIBLE VOIR VARIABLE DANS PARSING-) echo "HOME"'test'*'lol'$HOME'wow' 
// devient echo 'HOMEtest*lol$HOMEwow'.. CONCATENER $HOME DANS UNE STRING DANS 
// CONCAT_TO_LAST (creer new fonction) 

// echo "$44" doit rien afficher pas "4".