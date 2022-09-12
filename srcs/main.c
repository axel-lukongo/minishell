/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 17:08:01 by dasereno          #+#    #+#             */
/*   Updated: 2022/09/12 18:37:45 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	exec_tree(t_global *g)
{
	int	pid;

	if (g->ast && ((g->ast->type >= 0 && g->ast->type <= 3)
			|| (g->ast->type == 14) || (g->ast->type >= 20
				&& g->ast->type <= 24)))
	{
		if (g->ast->value && is_builtin(ft_split_quote(g->ast->value, ' ',
					g->alloc)[0]))
			execute_builtin(g, ft_split_quote(g->ast->value, ' ', g->alloc));
		else if (g->ast->value)
		{
			pid = fork();
			signal(SIGINT, SIG_IGN);
			if (pid == 0)
			{
				signal(SIGINT, sig);
				exec(ft_split_quote(g->ast->value, ' ', g->alloc), g->env, g);
			}
			else
				ft_waitpid(g, pid);
		}
	}
	else
		exec_ast(g->ast, g, NULL);
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
		expander(g->ast, g);
		if (g_p->error_cd == 1)
		{
			error_msg("cd", "HOME not set");
			g->ast = NULL;
			g_p->error_cd = 0;
		}
		g->last_return = 0;
		exec_tree(g);
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
	change_shlvl(g);
	push_ustack(g->dir_stack, get_value_by_name(g->env, "PWD"));
	while (1)
	{
		g->hered = 0;
		g->tmpfile = 0;
		signal(SIGINT, handle_signale_ctrl_c);
		signal(SIGQUIT, SIG_IGN);
		g->char_env = env_to_tab(g->env, &g->alloc);
		str = get_prompt_str(g);
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
	g.sh_pid = 3423;
	g.ast = NULL;
	g.lex = NULL;
	g.line = NULL;
	g.char_env = env;
	g.node_id = 0;
	g.writed = 0;
	g.sig_exited = 0;
	g.error = 0;
	g.hered = 0;
	g.tmpfile = 0;
	g_p = ft_malloc(sizeof(*g_p), &g.alloc);
	g_p->error_cd = 0;
	g.dir_stack = init_ustack(99, &g);
	loop(&g, env);
	ft_malloc_clear(&g.alloc);
	clear_history();
}
