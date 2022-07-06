/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/19 16:51:39 by alukongo          #+#    #+#             */
/*   Updated: 2022/07/06 20:56:48 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../include/minishell.h"

t_list *init_env(char **env)
{
	int i;
	char **strs;
	char *str;
	t_list *new_node;
	t_list *list_env;

	list_env = NULL;
	list_env = 0;
	i = 0;
	while (env[i])
	{
		str = ft_strdup(env[i]);
		if(!str)
			return(0);
		strs = ft_split(str, '=');
		new_node = ft_lstnew2(strs[0], strs[1]);
		if (!new_node)
			return(0);
		ft_lstadd_back(&list_env, new_node);
		i++;
	}
	return(list_env);
}

void	ft_env(t_list *env)
{
	int size;

	size = ft_lstsize(env);
	while (size)
	{
		ft_putstr_fd(env->content, 1);
		ft_putstr_fd(" = ", 1);
		ft_putstr_fd(env->result, 1);
		ft_putchar_fd('\n', 1);
		size--;
		if (size > 0)
			env = env->next;
	}
}
/*
int	main(int ac, char **av, char *env[])
{
	(void)ac;
	(void)av;
	t_list *my_env;
	if(!env || !*env)
		return(-1);
	my_env = init_env(env);
	while(my_env)
	{
		//printf("%s", (char *)my_env->content);
		//printf("=%s\n", (char *)my_env->result);
		my_env = my_env->next;
	}
}*/
