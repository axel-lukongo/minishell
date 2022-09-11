/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 16:06:33 by denissereno       #+#    #+#             */
/*   Updated: 2022/09/11 13:26:36 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	write_char(char *buffer, t_alloc **alloc, t_lex *lex)
{
	if ((buffer[lex->i] != '"' || lex->q.x != 1) && (buffer[lex->i] != '\''
			|| lex->q.y != 1) && buffer[lex->i])
	{
		lex->buf[lex->c++] = buffer[lex->i];
		expand_buf(&lex->buf_size, &lex->buf, lex->c, alloc);
		lex->buf[lex->c] = 0;
	}
}

void	first_wildcarded(char *buffer, t_alloc **alloc, t_lex *lex)
{
	if (get_btok(lex->t_lst, lex->k) == WILDCARD)
	{
		concat_to_last_no_space((t_token){WILDENV, lex->buf}, &lex->t_lst,
			alloc, 1);
	}
	else if (get_btok(lex->t_lst, lex->k) == CMD)
	{
		if (buffer[lex->i] == ' ')
			concat_to_last((t_token){WILDCARD, ft_strjoin(lex->buf, " ",
					alloc)}, &lex->t_lst, alloc, 1);
		else
			concat_to_last((t_token){WILDCARD, lex->buf},
				&lex->t_lst, alloc, 1);
	}
	else if (!lex->space)
	{
		concat_to_last_no_space((t_token){WILDCARD, lex->buf}, &lex->t_lst,
			alloc, 1);
	}
	else
		concat_to_last((t_token){WILDCARD, lex->buf}, &lex->t_lst,
			alloc, 1);
}

void	add_type(int type, t_lex *lex, t_alloc **alloc)
{
	add_to_list(type, lex->buf, &lex->t_lst, alloc);
	lex->k++;
}

void	concat_other(char *buffer, t_alloc **alloc, t_lex *lex)
{
	if (lex->enved)
	{
		if (get_btok(lex->t_lst, lex->k) == WILDCARD)
			concat_to_last_no_space((t_token){WILDENV, lex->buf}, &lex->t_lst,
				alloc, 1);
		else if (get_btok(lex->t_lst, lex->k) == CMD)
		{
			if (buffer[lex->i] == ' ')
				concat_to_last((t_token){ENV, ft_strjoin(lex->buf, " ", alloc)},
					&lex->t_lst, alloc, 1);
			else
				concat_to_last((t_token){ENV, lex->buf}, &lex->t_lst, alloc, 1);
		}
		else
			concat_to_last_no_space((t_token){ENV, lex->buf}, &lex->t_lst,
				alloc, 1);
	}
	else
		concat_to_last((t_token){0, lex->buf}, &lex->t_lst, alloc, 0);
}
