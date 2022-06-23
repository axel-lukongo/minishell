/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 21:57:29 by alukongo          #+#    #+#             */
/*   Updated: 2022/06/23 19:19:16 by alukongo         ###   ########.fr       */
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

void pipex(char **av)
{
 int fd[2];
 int pid1;
 int pid2;

 if(pipe(fd) == ERROR)
	return ;
 pid1 = fork(); 
 if (pid1 == 0)
 {
	dup2(fd[1], STDOUT_FILENO);
	close(fd[0]);
	close(fd[1]);
	execve("/bin/ls", ft_split(av[1], 0), NULL);
 }
 pid2 = fork();
 if (pid2 == 0)
 {
	dup2(fd[0], STDIN_FILENO);
	close(fd[1]);
	close(fd[0]);
	execve("/usr/bin/more", ft_split(av[2], 0), NULL);
 } 
 close(fd[1]);
 close(fd[0]);
 waitpid(pid1,NULL,0);
 waitpid(pid2,NULL,0);
}

int main(int ac, char **av)
{
	(void) ac;
	pipex(av);
}