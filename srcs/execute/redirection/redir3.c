/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 18:01:47 by denissereno       #+#    #+#             */
/*   Updated: 2022/09/11 18:02:01 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

t_token	**get_last_redir_one(t_alloc **alloc, t_tree *root)
{
	t_token	**tab;

	tab = ft_malloc(sizeof(t_token *) * 2, alloc);
	tab[0] = ft_malloc(sizeof(t_token *) * 2, alloc);
	if (root->right->type == WILDBACK || root->right->type == WILDCARD
		|| root->right->type == WILDENV || root->right->type == WILDENVBACK)
		tab[0]->str = ft_strdup(root->right->value, alloc);
	else
		tab[0]->str = ft_split_quote(root->right->value, ' ', *alloc)[0];
	tab[0]->type = root->type;
	tab[1] = NULL;
	return (tab);
}

t_token	**get_last_redir_several(t_alloc **alloc, t_vector2D tp, t_tree *r,
	int *i)
{
	t_token	**tab;

	tab = ft_malloc(sizeof(t_token **) * (1000), alloc);
	while (r->parent && (r->parent->type == tp.x || r->parent->type == tp.y))
	{
		tab[*i] = ft_malloc(sizeof(t_token *) * 2, alloc);
		if (r->right->type == WILDBACK || r->right->type == WILDCARD
			|| r->right->type == WILDENV || r->right->type == WILDENVBACK)
			tab[*i]->str = r->right->value;
		else
			tab[*i]->str = ft_split_quote(r->right->value, ' ', *alloc)[0];
		tab[*i]->type = r->type;
		r = r->parent;
		*i += 1;
	}
	tab[*i] = ft_malloc(sizeof(t_token *) * 2, alloc);
	if (r->right->type == WILDBACK || r->right->type == WILDCARD
		|| r->right->type == WILDENV || r->right->type == WILDENVBACK)
		tab[*i]->str = r->right->value;
	else
		tab[*i]->str = ft_split_quote(r->right->value, ' ', *alloc)[0];
	tab[*i]->type = r->type;
	return (tab);
}

t_token	**get_last_redir(t_tree *root, int type1, int type2, t_alloc **alloc)
{
	t_token	**tab;
	int		i;

	if ((!root->parent || (root->parent->type != type1 && root->parent->type
				!= type2)) && (root->type == type1 || root->type == type2))
		return (get_last_redir_one(alloc, root));
	else if (root->parent && (root->parent->type == type1
			|| root->parent->type == type2))
	{
		i = 0;
		tab = get_last_redir_several(alloc, (t_vector2D){type1, type2}, root,
				&i);
		i++;
		tab[i] = NULL;
		return (tab);
	}
	else
		return (NULL);
}

t_token	**get_last_redir_here_several(t_alloc **alloc, t_vector3D tp, t_tree *r,
	int *i)
{
	t_token	**tab;

	tab = ft_malloc(sizeof(t_token *) * (1000), alloc);
	while (r->parent && (r->parent->type == tp.x || r->parent->type == tp.y
			|| r->parent->type == tp.z))
	{
		tab[*i] = ft_malloc(sizeof(t_token *) * 2, alloc);
		if (r->right->type == WILDBACK || r->right->type == WILDCARD
			|| r->right->type == WILDENV || r->right->type == WILDENVBACK)
			tab[*i]->str = r->right->value;
		else
			tab[*i]->str = ft_split_quote(r->right->value, ' ', *alloc)
			[0];
		tab[*i]->type = r->type;
		r = r->parent;
		*i += 1;
	}
	tab[*i] = ft_malloc(sizeof(t_token *) * 2, alloc);
	if (r->right->type == WILDBACK || r->right->type == WILDCARD
		|| r->right->type == WILDENV || r->right->type == WILDENVBACK)
		tab[*i]->str = r->right->value;
	else
		tab[*i]->str = ft_split_quote(r->right->value, ' ', *alloc)[0];
	tab[*i]->type = r->type;
	return (tab);
}

t_token	**get_last_redir_here(t_tree *root, t_vector3D tp, t_alloc **alloc)
{
	t_token	**tab;
	int		i;

	if ((!root->parent || (root->parent->type != tp.x && root->parent->type
				!= tp.y && root->parent->type != tp.z))
		&& (root->type == tp.x || root->type == tp.y
			|| root->type == tp.y))
		return (get_last_redir_one(alloc, root));
	else if (root->parent && (root->parent->type == tp.x
			|| root->parent->type == tp.y || root->parent->type == tp.z))
	{
		i = 0;
		tab = get_last_redir_here_several(alloc, tp, root, &i);
		i++;
		tab[i] = NULL;
		return (tab);
	}
	else
		return (NULL);
}
