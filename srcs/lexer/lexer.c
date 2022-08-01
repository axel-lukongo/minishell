/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 17:24:47 by dasereno          #+#    #+#             */
/*   Updated: 2022/07/29 19:19:23 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	add_to_list(int type, char *str, t_list **li, t_alloc **alloc)
{
	t_token	*tk;

	tk = ft_malloc(sizeof(*tk), alloc);
	tk->str = ft_strdup(str, *alloc);
	tk->type = type;
	ft_lstadd_back(li, ft_lstnew((void *)tk, *alloc));
}

void	add_to_list_front(int type, char *str, t_list **li, t_alloc **alloc)
{
	t_token	*tk;

	tk = ft_malloc(sizeof(*tk), alloc);
	tk->str = ft_strdup(str, *alloc);
	tk->type = type;
	ft_lstadd_front(li, ft_lstnew((void *)tk, *alloc));
}

void	concat_to_last(t_token	tok, t_list **lst, t_alloc **alloc, int change_type)
{
	t_token	*tk;
	t_list	*li;

	li = *lst;
	while (li->next)
		li = li->next;
	tk = (t_token *)li->content;
	tk->str = ft_strjoin(tk->str, " ", alloc);
	tk->str = ft_strjoin(tk->str, tok.str, alloc);
	if (change_type)
		tk->type = tok.type;
	li = (void *)tk;
}

void	concat_to_last_no_space(t_token	tok, t_list **lst, t_alloc **alloc, int change_type)
{
	t_token	*tk;
	t_list	*li;

	li = *lst;
	while (li->next)
	{
		li = li->next;
	}
	tk = (t_token *)li->content;
	tk->str = ft_strjoin(tk->str, tok.str, alloc);
	if (change_type)
		tk->type = tok.type;
	li = (void *)tk;
}

int	ft_istoken(char c)
{
	if (c == '|' || c == ' ' || (c >= '\a' && c <= '\r') || c == '<'
		|| c == '>' || c == '&' || c == '(' || c == ')' || c == '*' || c == '\''
		|| c == '"')
		return (1);
	return (0);
}

int	ft_istoken_no_space(char c)
{
	if (c == '|' ||c == '<' || c == '>' || c == '&' || c == '(' || c == ')' || c == '*' || c == '\''
		|| c == '"')
		return (1);
	return (0);
}

int	ft_isspace(char c)
{
	if (c == ' ' || (c >= '\a' && c <= '\r'))
		return (1);
	return (0);
}

int	get_btok(t_list *li, int pos)
{
	int		i;
	t_token	*tk;

	i = 0;
	if (pos == 0)
		return (-1);
	else if (i > ft_lstsize(li))
		return (-1);
	while (i < pos - 1 && li)
	{
		li = li->next;
		i++;
	}
	if (!li)
		return (-1);
	tk = (t_token *)li->content;
	if (!tk)
		return (-1);
	return (tk->type);
}

// '(' -> '(' -> ')' -> ')' => ko no print
// '(' -> 'ls' -> ')' => ok
// '(' -> '(' -> 'ls' -> ')' -> ')' => ko no print

void	write_char(char *buffer, t_alloc **alloc, t_lex *lex)
{
	if ((buffer[lex->i] != '"' || lex->q.x != 1) && (buffer[lex->i] != '\'' || lex->q.y != 1) && buffer[lex->i] >= 0)
	{
		lex->buf[lex->c++] = buffer[lex->i];
		if (lex->c >= lex->buf_size)
		{
			lex->buf = ft_realloc(lex->buf, lex->buf_size, lex->buf_size * 2, alloc);
			lex->buf_size *= 2;
		}
		lex->buf[lex->c] = 0;
	}
}

void	first_wildcarded(char *buffer, t_alloc **alloc, t_lex *lex)
{
	if (get_btok(lex->t_lst, lex->k) == WILDCARD)
		concat_to_last_no_space((t_token){WILDENV, lex->buf}, &lex->t_lst, alloc, 1);
	else if (get_btok(lex->t_lst, lex->k) == CMD)
	{
		if (buffer[lex->i] == ' ')
			concat_to_last((t_token){WILDCARD, ft_strjoin(lex->buf, " ", alloc)}, &lex->t_lst, alloc, 1);
		else
			concat_to_last((t_token){WILDCARD, lex->buf}, &lex->t_lst, alloc, 1);
	}
	else
		concat_to_last_no_space((t_token){WILDCARD, lex->buf}, &lex->t_lst, alloc, 1);
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
			concat_to_last_no_space((t_token){WILDENV, lex->buf}, &lex->t_lst, alloc, 1);
		else if (get_btok(lex->t_lst, lex->k) == CMD)
		{
			if (buffer[lex->i] == ' ')
				concat_to_last((t_token){ENV, ft_strjoin(lex->buf, " ", alloc)}, &lex->t_lst, alloc, 1);
			else
				concat_to_last((t_token){ENV, lex->buf}, &lex->t_lst, alloc, 1);
		}
		else
			concat_to_last_no_space((t_token){ENV, lex->buf}, &lex->t_lst, alloc, 1);
	}
	else
		concat_to_last((t_token){0, lex->buf}, &lex->t_lst, alloc, 0);
}

void	buf_to_list(char *buffer, t_alloc **alloc, t_lex *lex)
{
	if (!(lex->i != 0 && (buffer[lex->i] == '"' || buffer[lex->i] == '\'') && buffer[lex->i - 1] != '"' && buffer[lex->i - 1] != '\'') && buffer[lex->i] != '*' && buffer[lex->i] != '\\')
	{
		if (lex->wildcarded == 1 && lex->start == 1)
			first_wildcarded(buffer, alloc, lex);
		else if (get_btok(lex->t_lst, lex->k) == GREAT || get_btok(lex->t_lst, lex->k) == DGREAT || get_btok(lex->t_lst, lex->k) == DLESS || get_btok(lex->t_lst, lex->k) == LESS)
			add_type(FILE, lex, alloc);
		else if (lex->start == 0 && lex->enved)
			add_type(ENV, lex, alloc);
		else if (lex->start == 0)
			add_type(CMD, lex, alloc);
		else
			concat_other(buffer, alloc, lex);
		lex->c = 0;
		lex->start = 1;
	}
}

void	buffer_writing(char *buffer, t_alloc **alloc, t_lex *lex)
{
	if (buffer[lex->i] == '\\')
		lex->backed = 1;
	if (((buffer[lex->i] == '$' && lex->quoted.x) || (buffer[lex->i] == '$' && !lex->quoted.x) || (buffer[lex->i] == '$' && !lex->quoted.y)))
		lex->enved = 1;
	if ((!ft_istoken(buffer[lex->i]) || ((lex->quoted.x && buffer[lex->i] != '"' && buffer[lex->i] != '\\') || (lex->quoted.y && buffer[lex->i] != '\''  && buffer[lex->i] != '\\'))))
		write_char(buffer, alloc, lex);
	else if ((ft_istoken(buffer[lex->i]) && lex->c != 0) && (!lex->quoted.y && (!lex->quoted.x || (lex->quoted.x && lex->enved))))
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

static void	end_dquote(char *buffer, t_alloc **alloc,  t_lex *lex)
{
	lex->buf[lex->c++] = '"';
	if (lex->c >= lex->buf_size)
	{
		lex->buf = ft_realloc(lex->buf, lex->buf_size, lex->buf_size * 2, alloc);
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

void	add_double_quote(char *buffer, t_alloc **alloc,  t_lex *lex)
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
			lex->buf = ft_realloc(lex->buf, lex->buf_size, lex->buf_size * 2, alloc);
			lex->buf_size *= 2;
		}
		lex->buf[lex->c] = 0;
	}
}

void	end_squote(char *buffer, t_alloc **alloc,  t_lex *lex)
{
	lex->buf[lex->c++] = '\'';
	if (lex->c >= lex->buf_size)
	{
		lex->buf = ft_realloc(lex->buf, lex->buf_size, lex->buf_size * 2, alloc);
		lex->buf_size *= 2;
	}
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
	printf("single\n");
	if (lex->q.y == 1)
		end_squote(buffer, alloc, lex);
	else
	{
		lex->quoted.y = 1;
		lex->buf[lex->c++] = '\'';
		if (lex->c >= lex->buf_size)
		{
			lex->buf = ft_realloc(lex->buf, lex->buf_size, lex->buf_size * 2, alloc);
			lex->buf_size *= 2;
		}
		lex->buf[lex->c] = 0;
		lex->q.y++;
	}
}

void	add_parent(t_alloc **alloc, t_lex *lex, char *s, int type)
{
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
	if (buffer[lex->i + 1] == ' ')
		concat_to_last((t_token){WILDCARD, "* "}, &lex->t_lst, alloc, 1);
	else
		concat_to_last((t_token){WILDCARD, "*"}, &lex->t_lst, alloc, 1);
}

int	add_wildcard(char *buffer, t_alloc **alloc, t_lex *lex)
{
	if (lex->q.y || lex->q.x)
	{
	}
	else if (lex->i == 0)
	{
		if (buffer[lex->i + 1] == ' ')
			add_to_list(WILDCARD, "* ", &lex->t_lst, alloc);
		else
			add_to_list(WILDCARD, "*", &lex->t_lst, alloc);
		lex->start = 1;
		lex->wildcarded = 1;
	}
	else if (lex->start == 0 || (lex->c != 0 && lex->start == 1) || (!ft_istoken(buffer[lex->i + 1]) && lex->start == 1))
	{
		lex->buf[lex->c++] = '*';
		lex->buf[lex->c] = 0;
		lex->wildcarded = 1;
	}
	else if (lex->start != 0)
		add_wildcard_not_start(buffer, alloc, lex);
	else
		return (0);
	return (1);
}

void	end_add_wildcarded_not_start(t_alloc **alloc, t_lex *lex)
{
	if (lex->enved && get_btok(lex->t_lst, lex->k) != CMD && !lex->backed)
		concat_to_last_no_space((t_token){WILDENV, lex->buf}, &lex->t_lst, alloc, 1);
	if (lex->enved && get_btok(lex->t_lst, lex->k) != CMD && lex->backed)
		concat_to_last_no_space((t_token){WILDENVBACK, lex->buf}, &lex->t_lst, alloc, 1);
	else if (lex->enved && get_btok(lex->t_lst, lex->k) == CMD && !lex->backed)
		concat_to_last((t_token){WILDENV, lex->buf}, &lex->t_lst, alloc, 1);
	else if (lex->enved && get_btok(lex->t_lst, lex->k) == CMD && lex->backed)
		concat_to_last((t_token){WILDENVBACK, lex->buf}, &lex->t_lst, alloc, 1);
	else if (get_btok(lex->t_lst, lex->k) != CMD && !lex->backed)
		concat_to_last_no_space((t_token){WILDCARD, lex->buf}, &lex->t_lst, alloc, 1);
	else if (get_btok(lex->t_lst, lex->k) != CMD && lex->backed)
		concat_to_last_no_space((t_token){WILDBACK, lex->buf}, &lex->t_lst, alloc, 1);
	else if (lex->backed)
		concat_to_last((t_token){WILDBACK, lex->buf}, &lex->t_lst, alloc, 1);
	else
		concat_to_last((t_token){WILDCARD, lex->buf}, &lex->t_lst, alloc, 1);
}

void	end_if_enved(t_alloc **alloc, t_lex *lex)
{
	if (get_btok(lex->t_lst, lex->k) == WILDCARD && !lex->backed)
		concat_to_last_no_space((t_token){WILDENV, lex->buf}, &lex->t_lst, alloc, 1);
	if (get_btok(lex->t_lst, lex->k) == WILDCARD && lex->backed)
		concat_to_last_no_space((t_token){WILDENVBACK, lex->buf}, &lex->t_lst, alloc, 1);
	else if (get_btok(lex->t_lst, lex->k) == CMD && !lex->backed)
		concat_to_last((t_token){ENV, lex->buf}, &lex->t_lst, alloc, 1);
	else if (get_btok(lex->t_lst, lex->k) == CMD && lex->backed)
		concat_to_last((t_token){BACKENV, lex->buf}, &lex->t_lst, alloc, 1);
	else if (lex->backed)
		concat_to_last_no_space((t_token){BACKENV, lex->buf}, &lex->t_lst, alloc, 1);
	else
		concat_to_last_no_space((t_token){ENV, lex->buf}, &lex->t_lst, alloc, 1);
}

void	end_other_option(t_alloc **alloc, t_lex *lex)
{
	if (lex->enved)
		end_if_enved(alloc, lex);
	else if (lex->backed && !lex->wildcarded && lex->start == 1)
		concat_to_last((t_token){BACKSLASH, lex->buf}, &lex->t_lst, alloc, 1);
	else if (lex->backed && !lex->wildcarded)
		concat_to_last_no_space((t_token){BACKSLASH, lex->buf}, &lex->t_lst, alloc, 1);
	else if (lex->backed && lex->wildcarded)
		concat_to_last_no_space((t_token){WILDBACK, lex->buf}, &lex->t_lst, alloc, 1);
	else
		concat_to_last((t_token){0, lex->buf}, &lex->t_lst, alloc, 0);
}

void	end_add_last(t_alloc **alloc, t_lex *lex)
{
	if (lex->wildcarded == 1 && lex->start == 1)
		end_add_wildcarded_not_start(alloc, lex);
	else if (get_btok(lex->t_lst, lex->k) == GREAT || get_btok(lex->t_lst, lex->k) == DGREAT || get_btok(lex->t_lst, lex->k) == DLESS || get_btok(lex->t_lst, lex->k) == LESS)
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

int	add_not_quoted_2(char *buffer, t_alloc **alloc, t_lex *lex)
{
	if (buffer[lex->i] == '<' && buffer[lex->i + 1] == '<' && !lex->q.x && !lex->q.y)
		add_redir("<<", alloc, lex, DLESS);
	else if (buffer[lex->i] == '&' && buffer[lex->i + 1] == '&' && !lex->q.x && !lex->q.y)
		add_binop("&&", alloc, lex, AND);
	else if (buffer[lex->i] == '&' && buffer[lex->i + 1] != '&' && !lex->q.x && !lex->q.y)
	{
		printf("bash: syntax error near unexpected token `&'\n");
		return (0);
	}
	else if (buffer[lex->i] == '(' && !lex->q.x && !lex->q.y)
		add_parent(alloc, lex, "(", LBRACE);
	else if (buffer[lex->i] == ')' && !lex->q.x && !lex->q.y)
		add_parent(alloc, lex, ")", RBRACE);
	return (1);
}

int	add_not_quoted(char *buffer, t_alloc **alloc, t_lex *lex) // AJOUTER LES SPECIAL CHAR A LA LISTE EN FONCTION DU TYPE
{
	if (buffer[lex->i] == '|' && buffer[lex->i + 1] != '|' && !lex->q.x && !lex->q.y)
	{
		add_to_list(PIPE, "|", &lex->t_lst, alloc);
		lex->start = 0;
		lex->k++;
		lex->c = 0;
		lex->enved = 0;
	}
	else if (buffer[lex->i] == '|' && buffer[lex->i + 1] == '|' && !lex->q.x && !lex->q.y)
		add_binop("||", alloc, lex, OR);
	else if (buffer[lex->i] == '>' && buffer[lex->i + 1] != '>' && !lex->q.x && !lex->q.y)
		add_redir(">", alloc, lex, GREAT);
	else if (buffer[lex->i] == '>' && buffer[lex->i + 1] == '>' && !lex->q.x && !lex->q.y)
		add_redir(">>", alloc, lex, DGREAT);
	else if (buffer[lex->i] == '<' && buffer[lex->i + 1] != '<' && !lex->q.x && !lex->q.y)
		add_redir("<", alloc, lex, LESS);
	else
	{
		if (!add_not_quoted_2(buffer, alloc, lex))
			return (0);
	}
	return (1);
}

int	lexing(char *buffer, t_alloc **alloc, t_lex *lex)
{
	while (buffer[lex->i])
	{
		buffer_writing(buffer, alloc, lex); // SI C'EST PAS UN CHARACTERE SPECIAL AJOUTER AU BUFFER
		if (!add_not_quoted(buffer, alloc, lex))
			return (0);
		else if (buffer[lex->i] == '"' && lex->q.y == 0) // SI CHAR EST DOUBLE QUOTE ET PAS ENTRE SIMPLE QUOTE
			add_double_quote(buffer, alloc, lex);
		else if (buffer[lex->i] == '\'' && lex->q.x == 0) // SI CHAR EST SIMPLE QUOTE ET PAS DANS DOUBLE QUOTE
			add_simple_quote(buffer, alloc, lex);
		if (buffer[lex->i] == '*') // SI CHAR EST WILDCARD
		{
			if (!add_wildcard(buffer, alloc, lex))
				return (0);
		}
		lex->i++;
	}
	if (lex->c != 0)
		end_add_last(alloc, lex); // SI IL RESTE ENCORE DES CHAR DANS LE BUFFER
	return (1);
}

int	check_error_lexer(t_lex *lex)
{
	if (lex->q.y != 0 || lex->q.x != 0)
	{
		printf("quote error\n");
		return (0);
	}
	if (lex->parent)
	{
		if (lex->parent > 0)
			printf("bash: syntax error near unexpected token `('\n");
		else
			printf("bash: syntax error near unexpected token `)'\n");
		return (0);
	}
	print_list(lex->t_lst);
	if (!lex->t_lst)
		return (0);
	return (1);
}

void	normalise_lex(t_lex *lex, t_alloc **alloc)
{
	add_to_list_front(LBRACE, "(", &lex->t_lst, alloc);
	add_to_list(RBRACE, ")", &lex->t_lst, alloc);
	add_to_list(END, "$", &lex->t_lst, alloc);
}

t_list	*lexer(char *buffer, t_alloc **alloc)
{
	t_lex	*lex;

	lex = ft_malloc(sizeof(*lex), alloc);
	*lex = (t_lex){NULL, NULL, 10, 0, 0, 0, 0, (t_vector2D){0, 0}, (t_vector2D){0, 0}, 0, 0, 0, 0};
	if (!buffer)
		return (NULL);
	lex->buf = ft_malloc(sizeof(char) * 10, alloc);
	if (!lexing(buffer, alloc, lex)) // ALGO
		return (NULL);
	if (!check_error_lexer(lex))
		return (NULL);
	normalise_lex(lex, alloc);
	return (lex->t_lst);
}

// UTILISER IS_BACKED POUR LES QUOTES

// CHECK ENCORE LE WILDENV A FOND

// SI $ dans une double lex->quoted print variable env.
// si $ dans une simple quote print litteralement le $...

// GERER TOUT LES PARSE ERROR DES REDIR. EXEMPLE : bash: syntax error near unexpected token `>>'



/*

	>>		---->		bash: syntax error near unexpected token `newline'
	<<   	---->		bash: syntax error near unexpected token `newline'
	<>   	---->		bash: syntax error near unexpected token `newline'
	<		---->		bash: syntax error near unexpected token `newline'
	>		---->		bash: syntax error near unexpected token `newline'

	<<<		---->		bash: syntax error near unexpected token `<'
	><   	---->		bash: syntax error near unexpected token `<'
	>><		---->		bash: syntax error near unexpected token `<'
	<><		---->		bash: syntax error near unexpected token `<'
	<<<>	---->		bash: syntax error near unexpected token `<'
	<<<>>	---->		bash: syntax error near unexpected token `<'

	>>>		---->		bash: syntax error near unexpected token `>'
	<>>		---->		bash: syntax error near unexpected token `>'
	<<><	---->		bash: syntax error near unexpected token `>'
	<<<>	---->		bash: syntax error near unexpected token `>'
	<<>		---->		bash: syntax error near unexpected token `>'
	>>><>	---->		bash: syntax error near unexpected token `>'

	>>>>	---->		bash: syntax error near unexpected token `>>'
	<<>>	---->		bash: syntax error near unexpected token `>>'

	<<<<	---->		bash: syntax error near unexpected token `<<'
	><<		---->		bash: syntax error near unexpected token `<<'
	>><<	---->		bash: syntax error near unexpected token `<<'
	<<<<<>	---->		bash: syntax error near unexpected token `<<'
	<<<<>	---->		bash: syntax error near unexpected token `<<'
	<<<<<	---->		bash: syntax error near unexpected token `<<'

	>><>	---->		bash: syntax error near unexpected token `<>'
	<<<>	---->		bash: syntax error near unexpected token `<>'

*/

// GERER PARENTHESE SYNTAX ERROR

// \\$HOME -> printf "\/user/....." \$HOME -> $HOME \\\$HOME -> $HOME

/*
echo " \n " -> \n
echo "\n" -> \n
echo " \\ " -> \
echo "\\" -> \
echo \n -> n
echo \\ -> \
echo "\"" -> "
echo \$HOME -> $HOME -> creer fonction dans expander qui chech tout les \ avant la var pour savoir
echo \\$HOME -> \/usr/....
echo \\\$HOME -> \$HOME -> creer fonction dans expander qui chech tout les \ avant la var pour savoir
*/