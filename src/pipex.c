/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 21:57:29 by alukongo          #+#    #+#             */
/*   Updated: 2022/06/27 16:46:13 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../include/minishell.h"


//-----------------------test---------------------//
/*
void redirection(int fd)
{
  int fd;

  fd = open(av[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
  dup2(fd, STDOUT_FILENO);
  printf("fd:%d\n",fd);
  execve("/bin/ls",&av[1],NULL);
}
*/
//lfp
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

void pipex(char **av, int pid1, int pid2, t_list *path)
{
 int fd[2];
 (void) path;

 if(pipe(fd) == ERROR)
	return ;
 pid1 = fork(); 
 if (pid1 == 0)
 {
	dup2(fd[1], STDOUT_FILENO);
	ft_close(fd);
	execve(g_data.valid_path[0], ft_split(av[1], 0), NULL);
 }
 pid2 = fork();
 if (pid2 == 0)
 {
	dup2(fd[0], STDIN_FILENO);
	ft_close(fd);
	execve(g_data.valid_path[1], ft_split(av[2], 0), NULL);
 } 
 ft_close(fd);
 waitpid(pid1,NULL,0);
 waitpid(pid2,NULL,0);
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
	pipex(av, pid1, pid2, my_list);
}