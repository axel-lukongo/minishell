/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_6.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/10 17:25:35 by denissereno       #+#    #+#             */
/*   Updated: 2022/09/11 15:13:43 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	less_only_2(t_alloc **alloc, t_lex *lex)
{
	if (get_btok(lex->t_lst, lex->k) >= LESS
		&& get_btok(lex->t_lst, lex->k) <= DGREAT)
	{
		lex->less_only = 0;
		lex->lessed = 1;
	}
	else
	{
		add_to_list_index(lex->k - 2, (t_token){CMD, lex->buf},
			&lex->t_lst, alloc);
		lex->less_only = 0;
		lex->c = 0;
		lex->buf[0] = 0;
		lex->k++;
		lex->lessed = 1;
	}
}

void	less_only_1(t_alloc **alloc, t_lex *lex)
{
	add_to_list(FILE, lex->buf, &lex->t_lst, alloc);
	lex->less_only = 2;
	lex->c = 0;
	lex->buf[0] = 0;
	lex->k++;
}

void	add_buf(t_alloc **alloc, t_lex *lex, char *buffer)
{
	if (lex->wildcarded == 1 && lex->start == 1)
		first_wildcarded(buffer, alloc, lex);
	else if (get_btok(lex->t_lst, lex->k) == GREAT || get_btok(lex->t_lst,
			lex->k) == DGREAT || get_btok(lex->t_lst, lex->k) == DLESS
		|| get_btok(lex->t_lst, lex->k) == LESS)
		add_type(FILE, lex, alloc);
	else if (lex->start == 0 && lex->enved)
		add_type(ENV, lex, alloc);
	else if (lex->start == 0)
		add_type(CMD, lex, alloc);
	else
		concat_other(buffer, alloc, lex);
	if (buffer[lex->i] != ' ')
		lex->space = 0;
	else
		lex->space = 1;
	lex->c = 0;
	lex->start = 1;
	lex->lessed = 0;
}

void	buf_to_list(char *buffer, t_alloc **alloc, t_lex *lex)
{
	if (lex->less_only == 1)
		less_only_1(alloc, lex);
	else if (lex->less_only == 2)
		less_only_2(alloc, lex);
	else if (!(lex->i != 0 && (buffer[lex->i] == '"' || buffer[lex->i] == '\'')
			&& buffer[lex->i - 1] != '"' && buffer[lex->i - 1] != '\'')
		&& buffer[lex->i] != '*' && buffer[lex->i] != '\\')
		add_buf(alloc, lex, buffer);
}
