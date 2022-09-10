/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/10 16:58:31 by denissereno       #+#    #+#             */
/*   Updated: 2022/09/10 17:05:06 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	parsing_call_heredoc(int ch, t_global *g)
{
	t_token	*node;

	if (ch == DLESS)
	{
		node = (t_token *)g_p->li->content;
		if (!node)
			return (0);
		g_p->hered = 1;
		here_doc_parser(node->str, g, g_p->tmpfile);
	}
	else if (ch == PIPE && g_p->hered)
	{
		g_p->tmpfile++;
		g_p->hered = 0;
	}
	return (1);
}

t_tree	*parse_redir(t_global *g)
{
	t_tree	*a;
	char	**str;
	int		ch;

	str = (char *[]){"<", "<<", ">", ">>", "|"};
	a = parse_word(g);
	while (1)
	{
		if (g_p->next_token->type >= LESS && g_p->next_token->type <= PIPE)
		{
			ch = g_p->next_token->type;
			if (!parsing_call_heredoc(ch, g))
				return (NULL);
			if (!create_exp_token_node(g_p->next_token->type,
					str[g_p->next_token->type - LESS], &a, g))
			{
				print_syntax_error(g, ch);
				return (NULL);
			}
		}
		else
			return (a);
	}
}

void	check_if_heredoc(void)
{
	if (g_p->hered)
	{
		g_p->tmpfile++;
		g_p->hered = 0;
	}
}
