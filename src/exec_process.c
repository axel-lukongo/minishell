/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 16:56:16 by alukongo          #+#    #+#             */
/*   Updated: 2022/06/27 17:13:32 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../include/minishell.h"

void ft_exec(char **av, int pid1)
{
 int fd[2];

 if(pipe(fd) == ERROR)
	return ;
 pid1 = fork(); 
 if (pid1 == 0)
 {
	//dup2(fd[1], STDOUT_FILENO);
	//ft_close(fd);
	execve(send_path(av[1]), ft_split(av[1], 0), NULL);
 }
 ft_close(fd);
 waitpid(pid1, NULL, 0);
}