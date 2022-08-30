/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 17:24:47 by dasereno          #+#    #+#             */
/*   Updated: 2022/08/30 14:37:54 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	add_not_quoted(char *b, t_alloc **alloc, t_lex *lex)
{
	if (b[lex->i] == '|' && b[lex->i + 1] != '|' && !lex->q.x && !lex->q.y)
	{
		add_to_list(PIPE, "|", &lex->t_lst, alloc);
		lex->start = 0;
		lex->k++;
		lex->c = 0;
		lex->enved = 0;
	}
	else if (b[lex->i] == '|' && b[lex->i + 1] == '|' && !lex->q.x && !lex->q.y)
		add_binop("||", alloc, lex, OR);
	else if (b[lex->i] == '>' && b[lex->i + 1] != '>' && !lex->q.x && !lex->q.y)
		add_redir(">", alloc, lex, GREAT);
	else if (b[lex->i] == '>' && b[lex->i + 1] == '>' && !lex->q.x && !lex->q.y)
		add_redir(">>", alloc, lex, DGREAT);
	else if (b[lex->i] == '<' && b[lex->i + 1] != '<' && !lex->q.x && !lex->q.y)
		add_redir("<", alloc, lex, LESS);
	else
	{
		if (!add_not_quoted_2(b, alloc, lex))
			return (0);
	}
	return (1);
}

int	lexing(char *buffer, t_alloc **alloc, t_lex *lex)
{
	while (buffer[lex->i])
	{
		buffer_writing(buffer, alloc, lex);
		if (!add_not_quoted(buffer, alloc, lex))
			return (0);
		else if (buffer[lex->i] == '"' && lex->q.y == 0)
			add_double_quote(buffer, alloc, lex);
		else if (buffer[lex->i] == '\'' && lex->q.x == 0)
			add_simple_quote(buffer, alloc, lex);
		if (buffer[lex->i] == '*')
		{
			if (!add_wildcard(buffer, alloc, lex))
				return (0);
		}
		if (lex->error)
			return (0);
		lex->i++;
	}
	if (lex->c != 0)
		end_add_last(alloc, lex);
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
	*lex = (t_lex){NULL, NULL, 10, 0, 0, 0, 0, (t_vector2D){0, 0},
		(t_vector2D){0, 0}, 0, 0, 0, 0, 0, 0};
	if (!buffer)
		return (NULL);
	lex->buf = ft_malloc(sizeof(char) * 10, alloc);
	if (!lexing(buffer, alloc, lex))
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

// GERER TOUT LES PARSE ERROR DES REDIR. EXEMPLE : bash: syntax error near 
// unexpected token `>>'

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
echo \$HOME -> $HOME -> creer fonction dans expander qui chech tout les \
avant la var pour savoir
echo \\$HOME -> \/usr/....
echo \\\$HOME -> \$HOME -> creer fonction dans expander qui chech tout les \
avant la var pour savoir
*/