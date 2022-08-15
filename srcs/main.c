/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 17:08:01 by dasereno          #+#    #+#             */
/*   Updated: 2022/08/15 16:20:21 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char 	*test;
char	*line;

void	copy_str(char dest[1024], char *src)
{
	int i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = 0;
}

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
		pr = ft_strjoin(pr, "✗ ", &g->alloc);
	}
	else 
	{
		pr = ft_strjoin(pr, UGRN, &g->alloc);
		pr = ft_strjoin(pr, "✓ ", &g->alloc);
	}
	pr = ft_strjoin(pr, BLUB, &g->alloc);
	pr = ft_strjoin(pr, ft_strjoin(g->disp_pwd, " ", &g->alloc), &g->alloc);
	pr = ft_strjoin(pr, YEL, &g->alloc);
	pr = ft_strjoin(pr , ft_strjoin("❯", reset, &g->alloc), &g->alloc);
	pr = ft_strjoin(pr, " ", &g->alloc);
	return (pr);
}

void	handle_signale_ctrl_c(int sig)
{
	//ici il faut clear tous ce qui a ete allouer
	write(1,"\n", 1);
	line = 0;
	ft_putstr_fd(test, 1);
	(void)sig;
}

int	loop(t_global *g)
{
	char		*str;
	
	signal(SIGINT, &handle_signale_ctrl_c);
	while (1)
	{
		str = get_prompt_str(g);
		test = str;
	//	signal(SIGQUIT, SIG_IGN);
		line = readline(str);
		printf("---> %s\n", str);
		g->lex = lexer(line, &g->alloc); // on creer notre liste de token
		g->ast = parsing(g->lex, g); // on parse et converti en arbre
		expander(g->ast, g); // on expand les *, $VAR et backslashs
		execute(g); // on execute l'arbre
		g->node_id = 0;
		free(line);
	} // REPARER LEXER CONCATENER AVEC WILDCARD ET ENV : echo "tetou"*"$HOME""salam"
	// REGARDER LE ECHO // //// //////.... REACTION BIZARRE A VOIR
}

int	main(int argc, char **argv, char **env)
{
	t_global	g;

	(void)argc;
	(void)argv;
	g.alloc = NULL;
	g.last_return = 0;
	g.env = init_env(env, g.alloc);
	//printf("variable du env: %s\n", (char *) g.env->content);
	//printf("variable du env: %s\n", (char *) g.env->result);
	g.sh_pid = getpid();
	g.ast = NULL;
	g.lex = NULL;
	g.line = NULL;
	g.char_env = env;
	g.node_id = 0;
	g.writed = 0;
	g.dir_stack = init_ustack(99, &g);
	push_ustack(g.dir_stack, get_value_by_name(g.env, "PWD"));
	loop(&g);
}

// MODIFIER FT_SPLIT POUR PAS QUIL SPLIT CQUI YA ENTRE QUOTE

// ESPACE PAS GERER POUR ECHO

// ARGS PARSING
// ""..."" <- 1 ARGS
// ""...""""..."" <- 2 ARGS
// "...""...." <- 1 ARGS
//