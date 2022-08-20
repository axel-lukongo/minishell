/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 16:31:24 by denissereno       #+#    #+#             */
/*   Updated: 2022/08/17 16:33:52 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_tree	*create_token_node(int type, t_global *g)
{
	t_tree	*tr;

	tr = new_tree((t_token){type, g_p->next_token->str}, g);
	g_p->next_token = (t_token *)g_p->li->content;
	g_p->li = g_p->li->next;
	return (tr);
}

t_tree	*left_brace_handling(t_tree *tr)
{
	if (g_p->next_token->type == RBRACE)
	{
		if (g_p->li)
		{
			g_p->next_token = (t_token *)g_p->li->content;
			g_p->li = g_p->li->next;
		}
		return (tr);
	}
	else
		return (NULL);
	return (NULL);
}

t_tree	*left_brace(t_tree *tr, t_global *g)
{
	g_p->next_token = (t_token *)g_p->li->content;
	g_p->li = g_p->li->next;
	tr = parse_op(g);
	if (!tr)
		return (NULL);
	return (left_brace_handling(tr));
}

t_tree	*create_exp_token_node(int type, char *str, t_tree **a, t_global *g)
{
	t_tree	*b;
	int		tk;

	tk = g_p->next_token->type;
	g_p->next_token = (t_token *)g_p->li->content;
	g_p->li = g_p->li->next;
	b = parse_word(g);
	if (!b)
		return (NULL);
	if (!*a && tk == PIPE)
		return (NULL);
	*a = join_tree(*a, b, (t_token){type, str}, g);
	return (*a);
}
