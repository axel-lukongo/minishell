/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 01:58:09 by alukongo          #+#    #+#             */
/*   Updated: 2022/08/26 12:00:22 by denissereno      ###   ########.fr       */
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
	print_list_export(cpy_env);
}

int	is_valid_identifier(char *str)
{
	int	i;

	i = 0;
	if (ft_strlen(str) == 0)
		return (0);
	if (str[0] >= '0' && str[0] <= '9')
		return (0);
	while (str[i])
	{
		if ((str[i] < 'A' || str[i] > 'Z') && (str[i] < 'a' || str[i] > 'z')
			&& (str[i] < '0' || str[i] > '9') && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

char	*del_start_end_char(char *str, char c, t_alloc **alloc)
{
	int		i;
	int		n;
	int		pos;
	char	*new;

	i = 0;
	n = 0;
	while (str[n] == c)
		n++;
	i = ft_strlen(str) - 1;
	while (str[i] == c)
	{
		i--;
		n++;
	}
	pos = i;
	new = ft_malloc((sizeof(char) * ft_strlen(str) - n + 1), alloc);
	n = 0;
	i = 0;
	while (str[n] == c)
		n++;
	while (n <= pos)
		new[i++] = str[n++];
	new[i] = 0;
	return (new);
}

void print_err(t_global *g, char **s, int i)
{
	printf("bash: export: `%s': not a valid identifier\n", s[i]);
	g->last_return = 1;
}

char    **split_two(char **new, char *str, char c, t_alloc **alloc)
{
        int     i;
        int     j;

        i = 0;
        j = 0;
        while (str[i] != c && str[i])
                i++;
        new = ft_malloc(sizeof(char *) * 3, alloc);
        new[0] = ft_malloc(sizeof(char) * (i + 1), alloc);
        new[1] = ft_malloc(sizeof(char) * (ft_strlen(str) - i + 1), alloc);
        i = 0;
        while (str[i] != c && str[i])
            new[0][j++] = str[i++];
		new[0][j] = 0;
        j = 0;
		i++;
        while (str[i])
            new[1][j++] = str[i++];
		new[1][j] = 0;
        new[2] = NULL;
        return (new);
}

char    **ft_split_first(char *str, char c, t_alloc **alloc)
{
        char    **new;
        int             i;

        i= 0;
        new = NULL;
        while (str[i] != c && str[i])
                i++;
        if (!str[i])
        {
                new = ft_malloc(sizeof(char *) * 2, alloc);
                new[0] = ft_strdup(str, *alloc);
                new[1] = NULL;
                return (new);
        }
        else
                return(split_two(new, str, c, alloc));
}

void	add_value_export(char **split, t_global *g)
{
	t_env	*node;

	node = ft_malloc(sizeof(*node), &g->alloc);
	node->name = split[0];
	node->value = NULL;
	if (!is_var_env_exist(g->export, split[0]))
		ft_lstadd_back(&g->export, ft_lstnew((void *){node}, g->alloc));
	g->last_return = 0;
}

void	add_value_env(char **split, t_global *g)
{
	t_env	*node;

	node = ft_malloc(sizeof(*node), &g->alloc);
	node->name = split[0];
	node->value = del_start_end_char(split[1], ' ', &g->alloc);
	change_value_or_add_it(g, &g->export, split[0], node->value);
	change_value_or_add_it(g, &g->env, split[0], node->value);
	g->last_return = 0;
}

void	export_utils(int i, char **cmd, t_global *g)
{
	char	**split;

	split = ft_split_first(cmd[i], '=', &g->alloc);
	if (split[0] && split[0][0] == '-')
	{
		printf("bash: export: %c%c: invalid option\n", split[0][0]
			, split[0][1]);
		g->last_return = 2;
	}
	else if (split[0] && !split[1] && is_valid_identifier(split[0]))
		add_value_export(split, g);
	else if (split[0] && split[1] && is_valid_identifier(split[0]))
		add_value_env(split, g);
	else 
		print_err(g, cmd, i);
}

void	my_export(t_global *g, char **cmd)
{
	int		i;

	i = 1;
	if	(!cmd[1])
	{
		my_aff_export(g->export);
		g->last_return = 0;
	}
	while (cmd[i])
		export_utils(i++, cmd, g);
}

void	my_unset(t_global *g, char **cmd)
{
	int		i;

	i = 1;
	while (cmd[i])
	{
		if (cmd[i] && cmd[i][0] == '-')
		{
			printf("bash: unset: %c%c: invalid option\n", cmd[i][0]
				, cmd[i][1]);
			g->last_return = 2;
		}
		else if (cmd[i] && is_valid_identifier(cmd[i]) && !is_char(cmd[i], '='))
		{
			destroy_env_var(&g->env, cmd[i]);
			destroy_env_var(&g->export, cmd[i]);
			g->last_return = 0;
		}
		else 
		{
			printf("bash: unset: `%s': not a valid identifier\n", cmd[i]);
			g->last_return = 1;
		}
		i++;
	}
}

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
		ft_putchar_fd('\n', 1);
		size--;
		if (size > 0)
			env = env->next;
	}
}
