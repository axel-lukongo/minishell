/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 16:49:59 by alukongo          #+#    #+#             */
/*   Updated: 2022/07/06 17:17:51 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../include/minishell.h"

/*void	redirection(char **av)
{
 int	fd;

  fd = open(av[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
  dup2(fd, STDOUT_FILENO);
  execve(send_path(av[1]),&av[1],NULL);
}*/


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