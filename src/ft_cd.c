/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 16:14:22 by alukongo          #+#    #+#             */
/*   Updated: 2022/07/06 20:40:08 by alukongo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../include/minishell.h"

static void error_msg(char *path)
{
	char *err;
	char *msg;
	
	err =  strerror(errno);
	msg = ft_strjoin(": ", err);
	ft_putstr_fd("mishell: cd: ", 1);
	ft_putstr_fd(path, 1);
	ft_putstr_fd(err, 1);
}



void ft_cd(char *path, t_list *env)
{
	char *old_path;
	char *current_path;
	int ret;
	
	old_path = my_find(env, "PWD");
	ret = chdir(path);
	if (ret == ERROR)
		error_msg(path);
	else
	{
		current_path = getcwd((char *)NULL, 0);
		set_env(env, "OLDPWD", old_path);
		set_env(env, "PWD", current_path);
	}
}