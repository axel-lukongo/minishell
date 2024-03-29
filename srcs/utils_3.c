/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/10 16:51:15 by denissereno       #+#    #+#             */
/*   Updated: 2022/09/13 11:36:38 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	convert_pwd_display(char dest[1024], char *pwd, t_list *env)
{
	int	i;
	int	offset;

	i = 0;
	offset = ft_cmptomax(pwd, get_value_by_name(env, "HOME"));
	if (offset == -1)
	{
		copy_str(dest, pwd);
		return ;
	}
	dest[i++] = '~';
	while (pwd[offset])
		dest[i++] = pwd[offset++];
	dest[i] = 0;
}

char	*get_prompt_str(t_global *g)
{
	char	*pr;

	pr = NULL;
	getcwd(g->pwd, 512);
	convert_pwd_display(g->disp_pwd, g->pwd, g->env);
	pr = ft_strjoin(pr, ft_strjoin(g->disp_pwd, " ", &g->alloc), &g->alloc);
	pr = ft_strjoin(pr, "> ", &g->alloc);
	return (pr);
}

void	handle_signale_ctrl_c(int sig)
{
	write(STDERR_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_p->last_return = 1;
	(void)sig;
}

void	sig(int sig)
{
	(void)sig;
}

void	ft_waitpid(t_global *g, int pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if ((WIFSIGNALED(status)))
	{
		g_p->last_return = 128 + WTERMSIG(status);
		if (WTERMSIG(status) != 3)
		{
			printf("\n");
			g->sig_exited = 1;
		}
	}
	else
		g_p->last_return = WEXITSTATUS(status);
}
