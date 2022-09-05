/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 17:23:32 by darian            #+#    #+#             */
/*   Updated: 2022/09/05 19:12:54 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_parsing	*g_p;

t_tree	*parse_redir(t_global *g)
{
	t_tree	*a;
	char	**str;
	int		ch;

	str = (char *[]){"<", "<<", ">", ">>", "|"};
	a = parse_word(g);
	while (1)
	{
		if (g_p->next_token->type >= LESS && g_p->next_token->type <= PIPE)
		{
			ch = g_p->next_token->type;
			if (!create_exp_token_node(g_p->next_token->type,
					str[g_p->next_token->type - LESS], &a, g))
			{
				if (ch == PIPE && !g->writed)
					printf("bash: syntax error near unexpected token `|'\n");
				else if (ch >= LESS && ch < PIPE && !g->writed)
					printf("bash: syntax error near unexpected token `newline'\n");
				g->writed = 1;
				return (NULL);
			}
		}
		else
			return (a);
	}
}

int	parse_binop(t_global *g, t_token type, t_tree **a, t_tree **b)
{
	g_p->next_token = (t_token *)g_p->li->content;
	g_p->li = g_p->li->next;
	*b = parse_redir(g);
	if (!(*b) || !(*a))
		return (0);
	if (!(*b) && !g->writed)
	{
		printf("bash: syntax error near unexpected token `%s'\n", type.str);
		g->writed = 1;
		return (0);
	}
	else if (!(*a) && !g->writed)
	{
		printf("bash: syntax error near unexpected token `%s'\n", type.str);
		g->writed = 1;
		return (0);
	}
	*a = join_tree(*a, *b, type, g);
	return (1);
}

t_tree	*parse_op(t_global *g)
{
	t_tree	*a;
	t_tree	*b;

	a = parse_redir(g);
	while (1)
	{
		if (g_p->next_token->type == OR)
		{
			if (!parse_binop(g, (t_token){OR, "||"}, &a, &b))
				return (NULL);
		}
		else if (g_p->next_token->type == AND)
		{
			if (!parse_binop(g, (t_token){AND, "&&"}, &a, &b))
				return (NULL);
		}
		else
			return (a);
	}
}

t_tree	*parse_word(t_global *g)
{
	if (!g_p->li)
		return (NULL);
	if (g_p->next_token->type == CMD)
		return (create_token_node(CMD, g));
	else if (g_p->next_token->type == WILDCARD)
		return (create_token_node(WILDCARD, g));
	else if (g_p->next_token->type == WILDENV)
		return (create_token_node(WILDENV, g));
	else if (g_p->next_token->type == WILDBACK)
		return (create_token_node(WILDBACK, g));
	else if (g_p->next_token->type == BACKENV)
		return (create_token_node(BACKENV, g));
	else if (g_p->next_token->type == BACKSLASH)
		return (create_token_node(BACKSLASH, g));
	else if (g_p->next_token->type == WILDENVBACK)
		return (create_token_node(WILDENVBACK, g));
	else if (g_p->next_token->type == ENV)
		return (create_token_node(ENV, g));
	else if (g_p->next_token->type == FILE)
		return (create_token_node(FILE, g));
	else if (g_p->next_token->type == LBRACE)
		return (left_brace(NULL, g));
	return (NULL);
}

t_tree	*parsing(t_list *li, t_global *g)
{
	t_tree	*tr;

	if (!li)
		return (NULL);
	g_p = ft_malloc(sizeof(*g_p), &g->alloc);
	g_p->li = li;
	g_p->next_token = (t_token *)li->content;
	g_p->li = g_p->li->next;
	tr = parse_op(g);
	g->writed = 0;
	g_p->error_cd = 0;
	if (g_p->next_token->type != END)
		return (NULL);
	return (tr);
}

// EXPLICATIONS ALGO :
/*Il y a plusieurs priorité:
	1 => && ||
	2 => >> << > < |
	3 => WORD (COMMANDE, FICHIER, *, VARIABLE ENVIRONNEMENT)

C'est un algo de recursive descent parsing ça veut dire que je commence par la 
plus haute priorité pour descendre vers le moins prioritaire
comme dans une expression mathématique ([+, -] prioritaire a [*, /] prioriataire
 au [nombres]).
les + et - sont prioritaires aux * et / car on va dabord calcuer les * et / 
autour des + et - pour ensuite les additionner ou soustraire.
exemple :
Si on veut parser "5 * 5 + 5 * 7" en priorité ca nous donnerai (5 * 5) + (5 * 7)
donc on regarde ce qu'il y a autour +,
on obtient
		25 + 35
puis
		   60

nous on parse dans un arbre sans calculer direct donc ca donnerai en gros:
					"5 * 5 + 5 * 7"
on parse en premier les additions/soustraction donc on cherche ce qui autours 
de ces derniers.
donc l'algo va chercher en premier le premier + et regarder a gauche puis a 
droite.

L'arbre au debut est : 
				(+)
on complete l'arbre a gauche avant. on tombe sur une multiplication donc on 
parse ce qu'il y a gauche et a droite de la multiplication
ce qui donne :
				(+)
			   /
			(*)

maintenant on regarde a droite et a gauche du *, a gauche on a a 5
				(+)
			   /
			(*)
		   /
		 (5)
maintenant a droite on a 5.
				(+)
			   /
			(*)
		   /   \
		 (5)    (5)
On a finis de parser la multiplication donc on revient en arriere et maintenant
 on va regarder a droite du + et on fait la meme chose, on tombe encore
sur une multiplication donc l'algo va encore descendre recursivement pour 
chercher a droite et gauche de cette derniere et on 
obtient : 
				(+)
			   /   \
			(*)		(*)
		   /   \    /  \
		 (5)    (5)(5)	(7)

et c'est la meme pour les commande bash, en gros "5 * 5 + 5 * 7" devient "ls 
| ls && echo lol > test.txt" l'arbre devient
				  (&&)
			   /       \
			(|)			(>)
		   /   \    	/  \
		 (cmd)  (cmd) (cmd)	(file)

Donc ça c'est la théorie maintenant en pratique il nous faut 3 fonctions pour 
les 3 priorités: 
	donc 1 pour les AND/OR une pour les redir/pipes et une pour les WORD

on va parser "ls | cat && echo lol"

Comme on a vu au tout debut la fonction commence par parser les AND et OR donc 
on rentre la fonction parse_op
parse_op a besoin d'une expresion composer de redir ou juste un mot a droite 
et a gauche.
Notre arbre dans parse_op au debut ressemble à ca
				(&&)
			  /
il attend l'expression de gauche donc on rentre dans parse_redir
				(&&)
			  /
			(|)
parse_redir attend un mot ou une redir/pipe a droite et a gauche de lui, la on 
a un mot a droite et a gauche donc
il va juste recup le mot de gauche en appelant parse_word
				(&&)
			  /
			(|)
		   /
		(ls)
puis celui de droite en appelant parse_word
				(&&)
			  /
			(|)
		   /  \
		(ls) (ls)
mainteant quil a tout recup il sort quitte la fonction et on revient dans la 
fonction parse_op stocké dans la pile recursive
et on parse l'expression de gauche
On rentre dans parse_redir mais il y a que un mot donc il rentre dans parse_word
				(&&)
			  /		\
			(|)		(echo lol)
		   /  \
		(ls) (ls)
il cherche si il y a un token redir ou pipes il en voit pas donc il return que 
le mot en lui meme et quitte sa fonction pour 
revenir recursivmenet sur parse_op
On a atteint le token de fin de commande donc on quitte le parsing avec l'arbre
créé.

*/