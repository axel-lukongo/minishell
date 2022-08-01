/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/22 17:23:32 by darian            #+#    #+#             */
/*   Updated: 2022/07/29 19:10:25 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_parsing	*p;

t_tree	*create_exp_token_node(int type, char *str, t_tree **a, t_global *g)
{
	t_tree	*b;
	int		tk;

	tk = p->next_token->type;
	p->next_token = (t_token *)p->li->content;
	p->li = p->li->next;
	b = parse_word(g);
	if (!b)
		return (NULL);
	if (!*a && tk == PIPE)
		return (NULL);
	*a = join_tree(*a, b, (t_token){type, str}, g);
	return (*a);
}

t_tree	*parse_redir(t_global *g) // PARSING DES REDIRS / PIPES <- PARSE_REDIR
{
	t_tree	*a;
	char	**str;
	int		ch;
	
	str = (char *[]){"<", "<<", ">", ">>", "|"};
	a = parse_word(g); // ON RECUPERER LE MOT A GAUCHE DE LA REDIRECTION 
	while (1)
	{
		if (p->next_token->type >= LESS && p->next_token->type <= PIPE) // SI C'EST UN REDIR OU PIPE
		{
			ch = p->next_token->type;
			if (!create_exp_token_node(p->next_token->type, str[p->next_token->type - LESS], &a, g))
			{
				if (ch == PIPE && !g->writed)
					printf("bash: syntax error near unexpected token `|'\n");
				else if (!g->writed)
					printf("bash: syntax error near unexpected token `|'\n");
				g->writed = 1;
				return (NULL);
			}
		}
		else // SI C'EST PAS UNE REDIR OU PIPE ON RETURN L'ARBRE DE GAUCHE (comme pour les op, si ya pas de redir alors on return ce qu'il y a gauche (car il n'y a rien a droite pck ya pas redir))
			return (a);
	}
}

t_tree	*parse_op(t_global *g) // PARSING DES OPERATEUR BINAIRES 
{
	t_tree	*a; // ARBRE DE GAUCHE
	t_tree	*b; // ARBRE DE DROITE

	a = parse_redir(g); // ON  RECUPERE LES REDIRS/PIPES QU'IL Y A GAUCHE DE L'OP BINAIRE
	while (1)
	{
		if (p->next_token->type == OR) // SI C'EST UN OR
		{
			p->next_token = (t_token *)p->li->content;
			p->li = p->li->next;
			b = parse_redir(g);
			if (!b && !g->writed)
			{
				printf("bash: syntax error near unexpected token `||'\n");
				g->writed = 1;
				return (NULL);
			}
			else if (!a && !g->writed)
			{
				printf("bash: syntax error near unexpected token `||'\n");
				g->writed = 1;
				return (NULL);
			}
			a = join_tree(a, b, (t_token){OR, "||"}, g);
		}
		else if (p->next_token->type == AND) // SI C'EST UN AND
		{
			p->next_token = (t_token *)p->li->content;
			p->li = p->li->next;
			b = parse_redir(g);
			if (!b && !g->writed)
			{
				printf("bash: syntax error near unexpected token `&&'\n");
				g->writed = 1;
				return (NULL);
			}
			else if (!a && !g->writed)
			{
				printf("bash: syntax error near unexpected token `&&'\n");
				g->writed = 1;
				return (NULL);
			}
			a = join_tree(a, b, (t_token){AND, "&&"}, g);
		}
		else // SI YA PAS D'OPERATEUR BINAIRE ALORS ON RECUPERER JUSTE L'ARBRE DE GAUCHE QU'ON A PARSE (pas besoin d'arbre de droite car il n'existe pas (il n'y a rien a gauche du && ou || car il n'existe pas))
			return (a);
	}
}

t_tree	*create_token_node(int type, t_global *g)
{
	t_tree *tr;

	tr = new_tree((t_token){type, p->next_token->str}, g);
	p->next_token = (t_token *)p->li->content;
	p->li = p->li->next;
	return (tr);
}

t_tree	*left_brace_handling(t_tree *tr)
{
	if (p->next_token->type == RBRACE)
	{
		if (p->li)
		{
			p->next_token = (t_token *)p->li->content;
			p->li = p->li->next;
		}
		return (tr);
	}
	else
		return (NULL);
	return (NULL);
}

t_tree	*parse_word(t_global *g) // PARSING DES MOTS <- PARSE_WORD
{
	t_tree	*tr;

	if (!p->li)
		return (NULL);
	if (p->next_token->type == CMD) // ON CHERCHE LE TYPE DU MOT POUR LE RETURN DANS PARSE_REDIR
		return (create_token_node(CMD, g));
	else if (p->next_token->type == WILDCARD)
		return (create_token_node(WILDCARD, g));
	else if (p->next_token->type == WILDENV)
		return (create_token_node(WILDENV, g));
	else if (p->next_token->type == WILDBACK)
		return (create_token_node(WILDBACK, g));
	else if (p->next_token->type == BACKENV)
		return (create_token_node(BACKENV, g));
	else if (p->next_token->type == BACKSLASH)
		return (create_token_node(BACKSLASH, g));
	else if (p->next_token->type == WILDENVBACK)
		return (create_token_node(WILDENVBACK, g));
	else if (p->next_token->type == ENV)
		return (create_token_node(ENV, g));
	else if (p->next_token->type == FILE)
		return (create_token_node(FILE, g));
	else if (p->next_token->type == LBRACE) // SI C'EST UNE PARENTHESE ALORS CA VEUT DIRE QU'ON DOIT PARSER UN OPERATEUR BINAIRE POTENTIELLEMENT DONC APPELLE LA FONCTION PARSE_OP
	{
		p->next_token = (t_token *)p->li->content;
		p->li = p->li->next;
		tr = parse_op(g);
		if (!tr)
			return (NULL);
		return (left_brace_handling(tr));
	}
	return (NULL);

}

t_tree	*parsing(t_list *li, t_global *g)
{
	t_tree	*tr;

	if (!li)
		return (NULL);
	p = malloc(sizeof(*p));
	p->li = li;
	p->next_token = (t_token *)li->content;
	p->li = p->li->next;
	tr = parse_op(g); // ON COMMANDE PAR PARSER LES OP BINAIRE
	g->writed = 0;
	if (p->next_token->type != END)
		return (NULL);
	return (tr);
}

// EXPLICATIONS ALGO :
/*
Il y a plusieurs priorité:
	1 => && ||
	2 => >> << > < |
	3 => WORD (COMMANDE, FICHIER, *, VARIABLE ENVIRONNEMENT)

C'est un algo de recursive descent parsing ça veut dire que je commence par la plus haute priorité pour descendre vers le moins prioritaire
comme dans une expression mathématique ([+, -] prioritaire a [*, /] prioriataire au [nombres]).
les + et - sont prioritaires aux * et / car on va dabord calcuer les * et / autour des + et - pour ensuite les additionner ou soustraire.
exemple :
Si on veut parser "5 * 5 + 5 * 7" en priorité ca nous donnerai (5 * 5) + (5 * 7)
donc on regarde ce qu'il y a autour +,
on obtient
		25 + 35
puis
		   60

nous on parse dans un arbre sans calculer direct donc ca donnerai en gros:
					"5 * 5 + 5 * 7"
on parse en premier les additions/soustraction donc on cherche ce qui autours de ces derniers.
donc l'algo va chercher en premier le premier + et regarder a gauche puis a droite.

L'arbre au debut est : 
				(+)
on complete l'arbre a gauche avant. on tombe sur une multiplication donc on parse ce qu'il y a gauche et a droite de la multiplication
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
On a finis de parser la multiplication donc on revient en arriere et maintenant on va regarder a droite du + et on fait la meme chose, on tombe encore
sur une multiplication donc l'algo va encore descendre recursivement pour chercher a droite et gauche de cette derniere et on 
obtient : 
				(+)
			   /   \
			(*)		(*)
		   /   \    /  \
		 (5)    (5)(5)	(7)

et c'est la meme pour les commande bash, en gros "5 * 5 + 5 * 7" devient "ls | ls && echo lol > test.txt" l'arbre devient
				  (&&)
			   /       \
			(|)			(>)
		   /   \    	/  \
		 (cmd)  (cmd) (cmd)	(file)

Donc ça c'est la théorie maintenant en pratique il nous faut 3 fonctions pour les 3 priorités: 
	donc 1 pour les AND/OR une pour les redir/pipes et une pour les WORD

on va parser "ls | cat && echo lol"

Comme on a vu au tout debut la fonction commence par parser les AND et OR donc on rentre la fonction parse_op
parse_op a besoin d'une expresion composer de redir ou juste un mot a droite et a gauche.
Notre arbre dans parse_op au debut ressemble à ca
				(&&)
			  /
il attend l'expression de gauche donc on rentre dans parse_redir
				(&&)
			  /
			(|)
parse_redir attend un mot ou une redir/pipe a droite et a gauche de lui, la on a un mot a droite et a gauche donc
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
mainteant quil a tout recup il sort quitte la fonction et on revient dans la fonction parse_op stocké dans la pile recursive
et on parse l'expression de gauche
On rentre dans parse_redir mais il y a que un mot donc il rentre dans parse_word
				(&&)
			  /		\
			(|)		(echo lol)
		   /  \
		(ls) (ls)
il cherche si il y a un token redir ou pipes il en voit pas donc il return que le mot en lui meme et quitte sa fonction pour 
revenir recursivmenet sur parse_op
On a atteint le token de fin de commande donc on quitte le parsing avec l'arbre créé.

*/