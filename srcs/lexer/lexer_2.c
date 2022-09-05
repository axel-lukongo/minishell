/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 16:06:33 by denissereno       #+#    #+#             */
/*   Updated: 2022/09/05 18:27:51 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	write_char(char *buffer, t_alloc **alloc, t_lex *lex)
{
	if ((buffer[lex->i] != '"' || lex->q.x != 1) && (buffer[lex->i] != '\''
			|| lex->q.y != 1) && buffer[lex->i])
	{
		lex->buf[lex->c++] = buffer[lex->i];
		if (lex->c >= lex->buf_size)
		{
			lex->buf = ft_realloc(lex->buf, lex->buf_size, lex->buf_size * 2,
					alloc);
			lex->buf_size *= 2;
		}
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



void	buf_to_list(char *buffer, t_alloc **alloc, t_lex *lex)
{
	if (lex->less_only == 1)
	{
		print_list(lex->t_lst);
		printf("%s, %d\n", lex->buf, get_btok(lex->t_lst, lex->k));
		add_to_list(FILE, lex->buf, &lex->t_lst
			, alloc);
		print_list(lex->t_lst);
		lex->less_only = 2;
		lex->c = 0;
		lex->buf[0] = 0;
		lex->k++;
	}
	else if (lex->less_only == 2)
	{
		if (get_btok(lex->t_lst, lex->k) >= LESS && get_btok(lex->t_lst, lex->k)  <= DGREAT)
		{
			lex->less_only = 0;
			lex->lessed = 1;
		}
		else
		{
			add_to_list_index(lex->k - 2, (t_token){CMD, lex->buf}, &lex->t_lst
				, alloc);
			lex->less_only = 0;
			lex->c = 0;
			lex->buf[0] = 0;
			lex->k++;
			lex->lessed = 1;
		}
	}
	else if (!(lex->i != 0 && (buffer[lex->i] == '"' || buffer[lex->i] == '\'')
			&& buffer[lex->i - 1] != '"' && buffer[lex->i - 1] != '\'')
		&& buffer[lex->i] != '*' && buffer[lex->i] != '\\')
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
}
