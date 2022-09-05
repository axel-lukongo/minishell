/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 17:08:01 by dasereno          #+#    #+#             */
/*   Updated: 2022/09/05 15:10:34 by denissereno      ###   ########.fr       */
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
	cmds = ft_split(g->line, ';', &g->alloc);
	while (cmds[i])
	{
		g->lex = lexer(cmds[i], &g->alloc);
		g->ast = parsing(g->lex, g);
		printTree(g->ast, NULL, 0);
		expander(g->ast, g);
		g->last_return = 0;
		exec_ast(g->ast, g, NULL);
		add_history(cmds[i]);
		g->node_id = 0;
		i++;
	}
}

/*

=> "> bonjour echo > test lol > oui"

      .——— (oui)
 ——— (>)
    |      .——— (test lol)
    `——— (>)
       |      .——— (bonjour)
       `——— (>)
          `——— (echo)

Si redirection en cours, prend le deuxieme argument de > si ya
et les ajouter a gauche de la redir dans l'arbre.

Exemple:
	"echo > test lol > oui"
	
          .——— (test)
    `——— (>)
		`——— (lol)	
       |      .——— (bonjour)
       `——— (>)
          `——— (echo)

Pendant le parsing de l'arbre pour l'execution, il faut concatanerer
toutes les partie a gauche d'un '>' et l'executer a la fin pour le rediriger
dans le dernier fichier.

*/

int	loop(t_global *g, char **env)
{
	char		*str;

	g->env = init_env(env, &g->alloc, 0);
	g->export = init_env(env, &g->alloc, 0);
	push_ustack(g->dir_stack, get_value_by_name(g->env, "PWD"));
	signal(SIGINT, &handle_signale_ctrl_c);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		str = get_prompt_str(g);
		if (!g->sig_exited)
			g->line = readline(str);
		else
		{
			g->line = readline(NULL);
			g->sig_exited = 0;
		}
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
	g.sh_pid = getpid();
	g.ast = NULL;
	g.lex = NULL;
	g.line = NULL;
	g.char_env = env;
	g.node_id = 0;
	g.writed = 0;
	g.sig_exited = 0;
	g.error = 0;
	g.dir_stack = init_ustack(99, &g);
	loop(&g, env);
	ft_malloc_clear(&g.alloc);
	clear_history();
}
