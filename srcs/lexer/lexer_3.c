/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_3.C                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 16:06:53 by denissereno       #+#    #+#             */
/*   Updated: 2022/09/10 17:35:01 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	buffer_writing(char *buffer, t_alloc **alloc, t_lex *lex)
{
	if (buffer[lex->i] == '\\')
	{
		if (buffer[lex->i + 1] == 0 && (lex->i == 0 || buffer[lex->i - 1]
				!= '\\'))
		{
			error_msg("\\", "backslash error");
			lex->error = 1;
			return ;
		}
		lex->backed = 1;
	}
	if (((buffer[lex->i] == '$' && lex->quoted.x) || (buffer[lex->i] == '$'
				&& !lex->quoted.x) || (buffer[lex->i] == '$'
				&& !lex->quoted.y)))
		lex->enved = 1;
	if (((!ft_istoken(buffer[lex->i]) || ((lex->quoted.x && buffer[lex->i]
						!= '"' && buffer[lex->i] != '\\') || (lex->quoted.y
						&& buffer[lex->i] != '\'' && buffer[lex->i] != '\\')))
			&& !(buffer[lex->i] == ' ' && lex->less_only == 1))
		|| (lex->less_only == 2 && (!ft_istoken(buffer[lex->i]))))
		write_char(buffer, alloc, lex);
	else if ((ft_istoken(buffer[lex->i]) && lex->c != 0) && (!lex->quoted.y
			&& (!lex->quoted.x || (lex->quoted.x && lex->enved))))
		buf_to_list(buffer, alloc, lex);
}

void	add_redir(char *s, t_alloc **alloc, t_lex *lex, int type)
{
	char	*r[4];

	r[0] = "<";
	r[1] = "<<";
	r[2] = ">";
	r[3] = ">>";
	if (get_btok(lex->t_lst, lex->k) >= LESS && get_btok(lex->t_lst, lex->k)
		<= DGREAT && type >= LESS && type <= DGREAT && lex->buf[0] <= 0)
	{
		printf("bash: syntax error near unexpected token `%s'\n",
			r[type - LESS]);
		lex->error = 1;
	}
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
