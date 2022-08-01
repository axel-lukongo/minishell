/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/28 19:58:40 by alukongo          #+#    #+#             */
/*   Updated: 2022/08/01 21:59:31 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../include/minishell.h"

void	sort_list(t_list **env)
{
	t_list	*current;
	t_list	*next;
	t_list	*next_next;
	int size_list;

	(void) next;
	(void) next_next;
	(void) current;
	current = *env;
	size_list = ft_lstsize(current);
	while (size_list)
	{
		size_list--;
	}
}

void my_export(t_list *env)
{
	
	ft_env(env);
}
