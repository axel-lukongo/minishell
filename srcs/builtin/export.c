/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 01:58:09 by alukongo          #+#    #+#             */
/*   Updated: 2022/08/15 16:12:36 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	my_aff_export(t_list *env)
{
	t_list	*cpy_env;
	int		size;
	
	size = ft_lstsize(env);
	cpy_env = NULL;
	cpy_env = ft_cpy_env(cpy_env, env, size);
	ft_list_sort(&cpy_env, cmp);
	print_list_env(cpy_env);
	//ft_env(cpy_env);
}

void	my_export(t_global *g, char **cmd)
{
	char	**env;
	t_env	*node;
	if	(!cmd[1])
	{
		my_aff_export(g->env);
		g->last_return = 0;
		return ;
	}
	else
	{
		printf("Ca marchera\n");
		// if (!count_char(cmd[1], '=') || cmd[1][0] == '=')
		// 	return ;
		// env = ft_split(cmd[1], '=', g->alloc);
		// node = ft_malloc(sizeof(*node), &g->alloc);
		// if (!env[1])
		// 	*node = (t_env){env[0], ""};
		// else
		// 	*node = (t_env){env[0], env[1]};
		// if (is_var_env_exist(g->env, env[0]))
		// 	change_value_by_name(g, env[0], node->value);
		// else
		// 	ft_lstadd_back(&g->env, ft_lstnew((void *){node}, g->alloc));
	}
}

// }


t_list	*ft_lstnew2(void *content, void *result)
{
	t_list	*ptr;

	ptr = malloc(sizeof(t_list));
	if (!ptr)
		return (0);
	ptr->next = 0;
	ptr->content = content;
	ptr->result = result;
	return (ptr);
}

t_list	*ft_cpy_env(t_list *dest, t_list *src, int src_size)
{
	t_list *list1;
	t_list *list2;
	
	list1 = dest;
	list2 = src;
	while (src_size)
	{
		list1 = ft_lstnew2(list2->content, list2->result);
		if (!list1)
			return(0);
		ft_lstadd_back(&dest, list1);
		list2 = list2->next;
		src_size--;
	}
	return (dest);
}

int	cmp(void *content, void *content_ref)
{
	size_t i;
	char *str;
	char *str2;

	str2 = ((t_env *)content_ref)->name;
	str = ((t_env *)content)->name;
	i = 0;
	while (i < ft_strlen(str) && i < ft_strlen(str2))
	{
		if (str[i] > str2[i])
			return (1);
		else if (str[i] < str2[i])
			return (-1);
		i++;
	}
	return (-1);
}

void	ft_list_sort(t_list **begin_list, int (*cmp)())
{
	t_list *current;
	void *next_list_content;
	current = *begin_list;
	while (current->next)
	{
		if ((*cmp)(current->content, current->next->content) > 0)
		{
			next_list_content = current->content;
			current->content = current->next->content;
			current->next->content = next_list_content;
			current = *begin_list; 
		}
		else
			current = current->next;
	}
}

void	ft_env(t_list *env)
{
	int	size;

	size = ft_lstsize(env);
	while (size)
	{
		printf("%s", (char *)env->content);
		//ft_putstr_fd((char *)env->content, 1);
	//	ft_putstr_fd(" = ", 1);
	//	ft_putstr_fd(env->result, 1);
		ft_putchar_fd('\n', 1);
		size--;
		if (size > 0)
			env = env->next;
	}
}


