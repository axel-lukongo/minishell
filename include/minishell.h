#ifndef MINISHELL_H
# define MINISHELL_H
# define SUCCESS 0
#include "../libft/libft.h"
#include "../ft_printf/ft_printf.h"
int				ft_echo(char **args);

typedef struct	s_env_list
{
	char *env;
	struct s_env_list *next;
}t_env_list;

#endif