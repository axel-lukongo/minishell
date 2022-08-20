/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 16:04:56 by denissereno       #+#    #+#             */
/*   Updated: 2022/08/17 16:12:22 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	concat_to_last_no_space(t_token	tok, t_list **lst,
	t_alloc **alloc, int change_type)
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
	if (c == '|' || c == '<' || c == '>' || c == '&' || c == '(' || c == ')'
		|| c == '*' || c == '\''
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
