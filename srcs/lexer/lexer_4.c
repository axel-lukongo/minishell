/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 16:07:29 by denissereno       #+#    #+#             */
/*   Updated: 2022/09/11 15:14:21 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	end_squote(char *buffer, t_alloc **alloc, t_lex *lex)
{
	lex->buf[lex->c++] = '\'';
	expand_buf(&lex->buf_size, &lex->buf, lex->c, alloc);
	lex->buf[lex->c] = 0;
	if (buffer[lex->i + 1] != '\'' && buffer[lex->i + 1] != '"')
		lex->quoted = (t_vector2D){0, 0};
	else if (buffer[lex->i + 1] == '\'')
		lex->quoted = (t_vector2D){0, 1};
	else if (buffer[lex->i + 1] == '"')
		lex->quoted = (t_vector2D){1, 0};
	lex->q.y--;
}

void	add_simple_quote(char *buffer, t_alloc **alloc, t_lex *lex)
{
	if (lex->q.y == 1)
		end_squote(buffer, alloc, lex);
	else
	{
		lex->quoted.y = 1;
		lex->buf[lex->c++] = '\'';
		expand_buf(&lex->buf_size, &lex->buf, lex->c, alloc);
		lex->buf[lex->c] = 0;
		lex->q.y++;
	}
}

void	add_parent(t_alloc **alloc, t_lex *lex, char *s, int type)
{
	if (type == RBRACE && get_btok(lex->t_lst, lex->k) == LBRACE)
	{
		error_msg("syntax error near unexpected token", "`)'");
		lex->error = 1;
	}
	if (type == LBRACE && ((get_btok(lex->t_lst, lex->k) >= 0
				&& get_btok(lex->t_lst, lex->k) <= 3) || (get_btok(lex->t_lst
					, lex->k) >= 20 && get_btok(lex->t_lst, lex->k) <= 24)))
	{
		error_msg("syntax error near unexpected token", "`('");
		lex->error = 1;
	}
	add_to_list(type, s, &lex->t_lst, alloc);
	if (type == LBRACE)
		lex->parent++;
	else
		lex->parent--;
	lex->k++;
	lex->c = 0;
}

void	add_wildcard_not_start(char *buffer, t_alloc **alloc, t_lex *lex)
{
	(void)buffer;
	if (lex->w_space || get_btok(lex->t_lst, lex->k) == CMD)
		concat_to_last((t_token){WILDCARD, "*"}, &lex->t_lst, alloc, 1);
	else
		concat_to_last_no_space((t_token){WILDCARD, "*"}, &lex->t_lst, alloc,
			1);
}

int	add_wildcard(char *buffer, t_alloc **alloc, t_lex *lex)
{
	if (lex->q.y || lex->q.x)
		return (1);
	change_wspace(lex, buffer);
	if (lex->i == 0)
	{
		if (buffer[lex->i + 1] == ' ')
			add_to_list(WILDCARD, "* ", &lex->t_lst, alloc);
		else
			add_to_list(WILDCARD, "*", &lex->t_lst, alloc);
		lex->start = 1;
		lex->wildcarded = 1;
	}
	else if (lex->start == 0 || (lex->c != 0 && lex->start == 1)
		|| (!ft_istoken(buffer[lex->i + 1]) && lex->start == 1))
	{
		lex->buf[lex->c++] = '*';
		expand_buf(&lex->buf_size, &lex->buf, lex->c, alloc);
		lex->buf[lex->c] = 0;
		lex->wildcarded = 1;
	}
	else if (lex->start != 0)
		add_wildcard_not_start(buffer, alloc, lex);
	else
		return (0);
	return (1);
}
