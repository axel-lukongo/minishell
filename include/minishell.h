#ifndef MINISHELL_H
# define MINISHELL_H
# define SUCCESS 0
# define ERROR -1
#include "../libft/libft.h"
#include "../ft_printf/ft_printf.h"
#include <unistd.h>
#include<stdio.h>
#include <fcntl.h>
int				ft_echo(char **args);
t_list			*init_env(char **env);

typedef struct	m_list
{
	char *my_path;
	char **tab_path;
	char **valid_path;
}my_list;

my_list g_data;

#endif