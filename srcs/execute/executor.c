/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 12:47:51 by denissereno       #+#    #+#             */
/*   Updated: 2022/07/31 10:33:55 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**convert_tree_to_cmd(t_tree	*tr, t_global *g)
{
	char	**cmd;

	if (!tr)
		return (NULL);
	cmd = ft_malloc(sizeof(char *) * 3, &g->alloc);
	if (tr->left)
		cmd[0] = tr->left->value;
	if (tr->right)
		cmd[1] = tr->right->value;
	cmd[2] = NULL;
	return (cmd);
}

void	executing(t_exec_con *exec, t_global *g)
{
	int		pid;
	int		status;

	if (!ft_strcmp(exec->exec[0]->cmd, "!") || !ft_strcmp(exec->exec[0]->cmd, ":"))
	{
		g->last_return = 0;
		return ;
	}
	if (exec->nb_cmd == 1)
	{
		pid = fork();
		if (pid == 0)
			pipex(exec->nb_cmd, exec->exec, g->env, g); 
		waitpid(pid, &status, 0);
	}
	else if (exec->nb_cmd <= 0)
		return ;
	else
	{
		pid = fork();
		if (pid == 0)
			pipex(exec->nb_cmd, exec->exec, g->env, g);
		waitpid(pid, &status, 0);
	}
	g->last_return = status;
}

int is_directory(char *str)
{
	if (!opendir(str))
		return(0);
	return (1);
}

void	execute(t_global *g)
{
	t_exec_con	*exec;
	t_tree		*tr;
    char        **cmd;

	tr = get_first_exec_node(g->ast); // on recuperer la premiere node a executer de l'arbre
	while (tr)
	{
		exec = get_exec_container(g, tr); // on converti la node de l'arbre en structure utilisable pour executer
		if (!exec->exec[0])
			break ;
        cmd = ft_split_quote(exec->exec[0]->cmd, ' ', g->alloc); // on split en supprimant les quotes
		if (cmd && is_directory(cmd[0])) // si la cmd est un fichier
		{
			printf("bash: %s: is a directory\n", cmd[0]);
			return ;
		}
		if ((exec->parent && exec->is_left == 0 && g->last_return != 0) || (exec->is_left == 1) || (exec->parent && exec->parent->type == AND) || (!exec->parent))
        {
            if (cmd && is_builtin(cmd[0])) // si c'est une builtin
                execute_builtin(g, cmd);
            else if (cmd) // sinon si c'est pas
			{
				if (!(exec->parent && exec->parent->type == AND && g->last_return != 0)) // on execute que si c'est soit le fils d'une node AND et que la val de retour d'avant permet l'execution ou si la node n'a pas de parent
			  		executing(exec, g);
			}
			else // sinon on quitte l'execution en cas d'erreur
				return ;
        } 
		if (exec->parent && exec->is_left == 0)
		{
			exec->parent->return_val = g->last_return;
			exec->parent->type = RESULT;
		} // on change le type de node du parent pour s'assurer de ne plus reutiliser cette partie de l'arbre
		delete_tree(g->ast, exec->node_id, &g->ast); // on supprime la node de l'arbre
		tr = get_first_exec_node(g->ast); // on recupere la prochaine node a executer
		if (!count_executable_nodes(g->ast)) // si il reste plus rien a executer on quitte.
			break ;
	}
}
