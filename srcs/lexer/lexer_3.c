/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_3.C                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 16:06:53 by denissereno       #+#    #+#             */
/*   Updated: 2022/08/17 16:07:12 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	buffer_writing(char *buffer, t_alloc **alloc, t_lex *lex)
{
	if (buffer[lex->i] == '\\')
		lex->backed = 1;
	if (((buffer[lex->i] == '$' && lex->quoted.x) || (buffer[lex->i] == '$'
				&& !lex->quoted.x) || (buffer[lex->i] == '$'
				&& !lex->quoted.y)))
		lex->enved = 1;
	if ((!ft_istoken(buffer[lex->i]) || ((lex->quoted.x && buffer[lex->i] != '"'
					&& buffer[lex->i] != '\\') || (lex->quoted.y
					&& buffer[lex->i] != '\'' && buffer[lex->i] != '\\'))))
		write_char(buffer, alloc, lex);
	else if ((ft_istoken(buffer[lex->i]) && lex->c != 0) && (!lex->quoted.y
			&& (!lex->quoted.x || (lex->quoted.x && lex->enved))))
		buf_to_list(buffer, alloc, lex);
}

void	add_redir(char *s, t_alloc **alloc, t_lex *lex, int type)
{
	add_to_list(type, s, &lex->t_lst, alloc);
	lex->k++;
	lex->c = 0;
	if (type == DGREAT || type == DLESS)
		lex->i++;
	lex->enved = 0;
}

void	add_binop(char *s, t_alloc **alloc, t_lex *lex, int type)
{
	add_to_list(type, s, &lex->t_lst, alloc);
	lex->start = 0;
	lex->k++;
	lex->i++;
	lex->c = 0;
	lex->enved = 0;
}

static void	end_dquote(char *buffer, t_alloc **alloc, t_lex *lex)
{
	lex->buf[lex->c++] = '"';
	if (lex->c >= lex->buf_size)
	{
		lex->buf = ft_realloc(lex->buf,
				lex->buf_size, lex->buf_size * 2, alloc);
		lex->buf_size *= 2;
	}
	lex->buf[lex->c] = 0;
	if (buffer[lex->i + 1] != '\'' && buffer[lex->i + 1] != '"')
		lex->quoted = (t_vector2D){0, 0};
	else if (buffer[lex->i + 1] == '\'')
		lex->quoted = (t_vector2D){0, 1};
	else if (buffer[lex->i + 1] == '"')
		lex->quoted = (t_vector2D){1, 0};
	lex->q.x--;
}

void	add_double_quote(char *buffer, t_alloc **alloc, t_lex *lex)
{
	printf("double\n");
	if (lex->q.x == 1 && !is_backed(buffer, lex->i - 1))
		end_dquote(buffer, alloc, lex);
	else
	{
		if (!is_backed(buffer, lex->i - 1))
		{
			lex->quoted.x = 1;
			lex->q.x++;
		}
		lex->buf[lex->c++] = '"';
		if (lex->c >= lex->buf_size)
		{
			lex->buf = ft_realloc(lex->buf, lex->buf_size,
					lex->buf_size * 2, alloc);
			lex->buf_size *= 2;
		}
		lex->buf[lex->c] = 0;
	}
}
