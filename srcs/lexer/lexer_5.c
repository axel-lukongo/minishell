/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 16:11:16 by denissereno       #+#    #+#             */
/*   Updated: 2022/08/30 14:50:20 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	end_add_wildcarded_not_start(t_alloc **alloc, t_lex *lex)
{
	printf("loul\n");
	if (lex->enved && get_btok(lex->t_lst, lex->k) != CMD && !lex->backed)
		concat_to_last_no_space((t_token){WILDENV,
			lex->buf}, &lex->t_lst, alloc, 1);
	if (lex->enved && get_btok(lex->t_lst, lex->k) != CMD && lex->backed)
		concat_to_last_no_space((t_token){WILDENVBACK,
			lex->buf}, &lex->t_lst, alloc, 1);
	else if (lex->enved && get_btok(lex->t_lst, lex->k) == CMD && !lex->backed)
		concat_to_last((t_token){WILDENV, lex->buf},
			&lex->t_lst, alloc, 1);
	else if (lex->enved && get_btok(lex->t_lst, lex->k) == CMD && lex->backed)
		concat_to_last((t_token){WILDENVBACK, lex->buf},
			&lex->t_lst, alloc, 1);
	else if (get_btok(lex->t_lst, lex->k) != CMD && !lex->backed && !lex->space)
	{
		concat_to_last_no_space((t_token){WILDCARD,
			lex->buf}, &lex->t_lst, alloc, 1);
		printf("test\n");
	}
	else if (get_btok(lex->t_lst, lex->k) != CMD && lex->backed && !lex->space)
	{
		concat_to_last_no_space((t_token){WILDBACK,
			lex->buf}, &lex->t_lst, alloc, 1);
	}
	else if (get_btok(lex->t_lst, lex->k) >= 9 && get_btok(lex->t_lst, lex->k) <= 13)
		add_to_list(WILDCARD, lex->buf, &lex->t_lst, alloc);
	else if (lex->backed)
		concat_to_last((t_token){WILDBACK, lex->buf},
			&lex->t_lst, alloc, 1);
	else
		concat_to_last((t_token){WILDCARD, lex->buf},
			&lex->t_lst, alloc, 1);
}

void	end_if_enved(t_alloc **alloc, t_lex *lex)
{
	printf("la\n");
	if (get_btok(lex->t_lst, lex->k) == WILDCARD && !lex->backed)
		concat_to_last_no_space((t_token){WILDENV,
			lex->buf}, &lex->t_lst, alloc, 1);
	if (get_btok(lex->t_lst, lex->k) == WILDCARD && lex->backed)
		concat_to_last_no_space((t_token){WILDENVBACK,
			lex->buf}, &lex->t_lst, alloc, 1);
	else if (get_btok(lex->t_lst, lex->k) == CMD && !lex->backed)
		concat_to_last((t_token){ENV, lex->buf},
			&lex->t_lst, alloc, 1);
	else if (get_btok(lex->t_lst, lex->k) == CMD && lex->backed)
		concat_to_last((t_token){BACKENV, lex->buf},
			&lex->t_lst, alloc, 1);
	else if (lex->backed)
		concat_to_last_no_space((t_token){BACKENV,
			lex->buf}, &lex->t_lst, alloc, 1);
	else
	{
		if (lex->space)
			concat_to_last((t_token){ENV,
				lex->buf}, &lex->t_lst, alloc, 1);
		else
			concat_to_last_no_space((t_token){ENV,
				lex->buf}, &lex->t_lst, alloc, 1);
	}
}

void	end_other_option(t_alloc **alloc, t_lex *lex)
{
	if (lex->enved)
		end_if_enved(alloc, lex);
	else if (lex->backed && !lex->wildcarded && lex->start == 1)
		concat_to_last((t_token){BACKSLASH, lex->buf}, &lex->t_lst, alloc, 1);
	else if (lex->backed && !lex->wildcarded)
		concat_to_last_no_space((t_token){BACKSLASH,
			lex->buf}, &lex->t_lst, alloc, 1);
	else if (lex->backed && lex->wildcarded)
		concat_to_last_no_space((t_token){WILDBACK,
			lex->buf}, &lex->t_lst, alloc, 1);
	else
		concat_to_last((t_token){0, lex->buf}, &lex->t_lst, alloc, 0);
}

void	end_add_last(t_alloc **alloc, t_lex *lex)
{
	if (lex->wildcarded == 1 && lex->start == 1)
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

int	add_not_quoted_2(char *b, t_alloc **alloc, t_lex *lex)
{
	if (b[lex->i] == '<' && b[lex->i + 1] == '<' && !lex->q.x && !lex->q.y)
		add_redir("<<", alloc, lex, DLESS);
	else if (b[lex->i] == '&' && b[lex->i + 1] == '&' && !lex->q.x && !lex->q.y)
		add_binop("&&", alloc, lex, AND);
	else if (b[lex->i] == '&' && b[lex->i + 1] != '&' && !lex->q.x && !lex->q.y)
	{
		printf("bash: syntax error near unexpected token `&'\n");
		return (0);
	}
	else if (b[lex->i] == '(' && !lex->q.x && !lex->q.y)
		add_parent(alloc, lex, "(", LBRACE);
	else if (b[lex->i] == ')' && !lex->q.x && !lex->q.y)
		add_parent(alloc, lex, ")", RBRACE);
	return (1);
}
