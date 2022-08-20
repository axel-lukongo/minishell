/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/17 16:05:49 by denissereno       #+#    #+#             */
/*   Updated: 2022/08/17 16:06:05 by denissereno      ###   ########.fr       */
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

void	concat_to_last(t_token	tok, t_list **lst,
	t_alloc **alloc, int change_type)
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
