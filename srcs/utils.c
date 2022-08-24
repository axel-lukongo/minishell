/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 17:56:01 by dasereno          #+#    #+#             */
/*   Updated: 2022/08/22 16:16:36 by denissereno      ###   ########.fr       */
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
	t_token	*lol;

	while (t_lst)
	{
		lol = (t_token *)t_lst->content;
		printf("[%d]", lol->type);
		if (lol->str)
			printf(" -> %s\n", lol->str);
		t_lst = t_lst->next;
	}
}

void	print_list_export(t_list *li)
{
	t_env	*env;

	while (li)
	{
		env = (t_env *)li->content;
		if (env->value)
			printf("export %s=\"%s\"\n", env->name, env->value);
		else
			printf("export %s\n", env->name);
		li = li->next;
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

void	copy_str(char dest[1024], char *src)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = 0;
}

t_list	*init_env(char **env, t_alloc *alloc, int i)
{
	char	**strs;
	char	*str;
	t_list	*new_node;
	t_list	*list_env;
	t_env	*node;

	list_env = 0;
	while (env[i])
	{
		str = ft_strdup(env[i], alloc);
		if (!str)
			return (0);
		strs = ft_split(str, '=', alloc);
		node = ft_malloc(sizeof(*node), &alloc);
		*node = (t_env){strs[0], strs[1]};
		new_node = ft_lstnew((void *){node}, alloc);
		if (!new_node)
			return (0);
		ft_lstadd_back(&list_env, new_node);
		i++;
	}
	return (list_env);
}
