/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 17:08:01 by dasereno          #+#    #+#             */
/*   Updated: 2022/08/30 17:37:42 by denissereno      ###   ########.fr       */
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
	if (g->last_return > 0)
	{
		pr = ft_strjoin(pr, URED, &g->alloc);
		pr = ft_strjoin(pr, ft_itoa(g->last_return, g->alloc), &g->alloc);
		pr = ft_strjoin(pr, " ", &g->alloc);
	}
	else
	{
		pr = ft_strjoin(pr, UGRN, &g->alloc);
		pr = ft_strjoin(pr, ft_itoa(g->last_return, g->alloc), &g->alloc);
		pr = ft_strjoin(pr, " ", &g->alloc);
	}
	pr = ft_strjoin(pr, BLUB, &g->alloc);
	pr = ft_strjoin(pr, ft_strjoin(g->disp_pwd, " ", &g->alloc), &g->alloc);
	pr = ft_strjoin(pr, YEL, &g->alloc);
	pr = ft_strjoin(pr, ft_strjoin("❯", reset, &g->alloc), &g->alloc);
	pr = ft_strjoin(pr, " ", &g->alloc);
	return (pr);
}

void	handle_signale_ctrl_c(int sig)
{
	write(STDERR_FILENO, "\n", 1);
	rl_replace_line("", 0); 
	rl_on_new_line();
	rl_redisplay();
	(void)sig;
}

void	exec_line(t_global *g)
{
	char	**cmds;
	int		i;

	i = 0;
	cmds = ft_split(g->line, ';', g->alloc);
	while (cmds[i])
	{
		g->lex = lexer(cmds[i], &g->alloc);
		g->ast = parsing(g->lex, g);
		expander(g->ast, g);
		exec_ast(g->ast, g);
		g->node_id = 0;
		i++;
	}
}

int	loop(t_global *g)
{
	char		*str;

	while (1)
	{
		signal(SIGINT, &handle_signale_ctrl_c);
		str = get_prompt_str(g); // SI ON SORT D'UNE FONCTION EN CTRL C ON REDISPLAY PAS LA LIGNE
		g->line = readline(str);
		if (!g->line)
			return (0);
		exec_line(g);
		free(g->line);
	}
}

int	main(int argc, char **argv, char **env)
{
	t_global	g;

	(void)argc;
	(void)argv;
	g.alloc = NULL;
	g.last_return = 0;
	g.env = init_env(env, g.alloc, 0);
	g.sh_pid = getpid();
	g.ast = NULL;
	g.lex = NULL;
	g.line = NULL;
	g.char_env = env;
	g.node_id = 0;
	g.writed = 0;
	g.export = init_env(env, g.alloc, 0);
	g.dir_stack = init_ustack(99, &g);
	push_ustack(g.dir_stack, get_value_by_name(g.env, "PWD"));
	loop(&g);
}
