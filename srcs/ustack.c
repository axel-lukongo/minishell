/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ustack.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 17:47:50 by denissereno       #+#    #+#             */
/*   Updated: 2022/08/19 14:52:20 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	push_ustack(t_ustack *stack, char *value)
{
	int	i;

	i = 0;
	pop_ustack(stack, value, 0, 1);
	while (stack->stack[i])
		i++;
	while (i)
	{
		stack->stack[i] = stack->stack[i - 1];
		i--;
	}
	stack->stack[0] = value;
}

void	clear_ustack(t_ustack *stack)
{
	int	i;

	i = 0;
	while (i < stack->stack_size)
		stack->stack[i++] = NULL;
}

t_ustack	*init_ustack(int size, t_global *g)
{
	t_ustack	*stack;

	stack = ft_malloc(sizeof(*stack), &g->alloc);
	stack->stack_size = size;
	stack->stack = ft_malloc(sizeof(char *) * (stack->stack_size + 1),
			&g->alloc);
	ft_memset(stack->stack, 0, sizeof(char *) * (stack->stack_size + 1));
	return (stack);
}

char	*get_value_ustack(t_ustack *st, int index)
{
	if (index > st->stack_size)
		return (NULL);
	return (st->stack[index]);
}
