/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 17:56:01 by dasereno          #+#    #+#             */
/*   Updated: 2022/07/08 13:52:19 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_cmptomax(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (0);
	while (s1[i] == s2[i] && s1[i] && s2[i])
		i++;
	if (s2[i])
		return (-1);
	return (i);
}

void	print_list(t_list *t_lst)
{
	while (t_lst)
	{
		t_token *lol = (t_token *)t_lst->content;
		printf("[%d]", lol->type);
		if (lol->str)
			printf(" -> %s\n", lol->str);
		t_lst = t_lst->next;
	}
}

void	print_list_env(t_list *li)
{
	t_env	*env;

	while (li)
	{
		env = (t_env *)li->content;
		printf("%s=%s\n", env->name, env->value);
		li = li->next;
	}
}
