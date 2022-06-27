/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_function.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 21:57:29 by alukongo          #+#    #+#             */
/*   Updated: 2022/06/27 17:14:13 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../include/minishell.h"

void ft_close(int *fd)
{
	close(fd[0]);
	close(fd[1]);
}

void find_good_path(int ac, char **av)
{
	int	i;
	int	j;
	char *commande;

	commande = NULL;
	i = 0;
	g_data.valid_path = malloc(sizeof(char *) * ac);
	while (i < ac - 1)
	{
		j = 0;
		commande = ft_strjoin(g_data.tab_path[j], av[i + 1]);
		while (g_data.tab_path[++j] && access(commande, 0) == -1)
			commande = ft_strjoin(g_data.tab_path[j], av[i + 1]);
		if(access(commande, 0) == 0)
			g_data.valid_path[i] = commande;
		i++;
	}
}

void find_path(t_list *my_list)
{
	int i;

	i = 0;
	while (ft_strcmp(my_list->content, "PATH") != 0)
			my_list = my_list->next;
	g_data.my_path = (char *)my_list->result;
	g_data.tab_path = ft_split(g_data.my_path, ':');
	while (g_data.tab_path[i])
	{
		g_data.tab_path[i] = ft_strjoin(g_data.tab_path[i], "/");
		i++;
	}
}

char	*send_path(char *commande)
{
	int	i;
	char *str;
	
	i = 0;
	while (g_data.tab_path[i])
	{
		str = ft_strjoin(g_data.tab_path[i], commande);
		if(access(str, 0) == 0)
			return(str);
		i++;
	}
	return (NULL);
}

int main(int ac, char **av, char *env[])
{
	int pid1;
	int pid2;
	t_list *my_list;

	pid1 = 0;
	pid2 = 0;
	my_list = init_env(env);
	find_path(my_list);
	find_good_path(ac, av);
	ft_exec(av, pid1);
	//pipex(av, pid1, pid2, my_list);
}