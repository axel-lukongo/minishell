/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ustack_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/19 14:52:30 by denissereno       #+#    #+#             */
/*   Updated: 2022/08/19 14:52:39 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_ustack(t_ustack *stack)
{
	int	i;

	i = 0;
	while (i < stack->stack_size && stack->stack[i])
	{
		printf("[%d]: %s\n", i, stack->stack[i]);
		i++;
	}
}

int	is_in_ustack(t_ustack *stack, char *value)
{
	int	i;

	i = 0;
	while (i < stack->stack_size)
	{
		if (!ft_strcmp(stack->stack[i], value))
			return (i);
		i++;
	}
	return (-1);
}

void	pop_index(int index, t_ustack *stack, int i)
{
	if (index > stack->stack_size)
		return ;
	while (index != i && i < stack->stack_size)
		i++;
	while (i + 1 < stack->stack_size)
	{
		stack->stack[i] = stack->stack[i + 1];
		i++;
	}
	stack->stack[stack->stack_size - 1] = 0;
}

void	pop_ustack(t_ustack *stack, char *value, int index, int type)
{
	int	i;
	int	del;

	del = 0;
	i = 0;
	if (type == 0)
		pop_index(index, stack, i);
	else
	{
		while (ft_strcmp(value, stack->stack[i]) && i < stack->stack_size - 1)
			i++;
		if (i != stack->stack_size - 1)
			del = 1;
		while (i + 1 < stack->stack_size)
		{
			stack->stack[i] = stack->stack[i + 1];
			i++;
		}
		if (del)
			stack->stack[stack->stack_size - 1] = 0;
	}
}
