/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/10 17:38:30 by denissereno       #+#    #+#             */
/*   Updated: 2022/09/11 15:14:25 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	less_only_end(t_alloc **alloc, t_lex *lex)
{
	add_to_list_index(lex->k - 2, (t_token){FILE, lex->buf}, &lex->t_lst,
		alloc);
	lex->less_only = 0;
	lex->c = 0;
	lex->buf[0] = 0;
	lex->k++;
}

void	end_add_last(t_alloc **alloc, t_lex *lex)
{
	if (lex->less_only == 2 && !(get_btok(lex->t_lst, lex->k) >= LESS
			&& get_btok(lex->t_lst, lex->k) <= DGREAT))
		less_only_end(alloc, lex);
	else if (lex->wildcarded == 1 && lex->start == 1)
		end_add_wildcarded_not_start(alloc, lex);
	else if (get_btok(lex->t_lst, lex->k) == GREAT || get_btok
		(lex->t_lst, lex->k) == DGREAT || get_btok(lex->t_lst, lex->k) == DLESS
		|| get_btok(lex->t_lst, lex->k) == LESS)
		add_to_list(FILE, lex->buf, &lex->t_lst, alloc);
	else if (lex->start == 0 && lex->enved && !lex->backed)
		add_to_list(ENV, lex->buf, &lex->t_lst, alloc);
	else if (lex->start == 0 && lex->enved && lex->backed)
		add_to_list(BACKENV, lex->buf, &lex->t_lst, alloc);
	else if (lex->start == 0 && !lex->backed)
		add_to_list(CMD, lex->buf, &lex->t_lst, alloc);
	else if (lex->start == 0 && lex->backed)
		add_to_list(BACKSLASH, lex->buf, &lex->t_lst, alloc);
	else
		end_other_option(alloc, lex);
	lex->k++;
}

void	expand_buf(int *buf_size, char **buf, int n, t_alloc **alloc)
{
	if (n >= *buf_size)
	{
		*buf = ft_realloc(*buf, *buf_size, *buf_size * 2, alloc);
		*buf_size = *buf_size * 2;
	}
}

void	change_wspace(t_lex *lex, char *buffer)
{
	if (lex->i - 1 && buffer[lex->i - 1] == ' ')
		lex->w_space = 1;
	else
		lex->w_space = 0;
}
