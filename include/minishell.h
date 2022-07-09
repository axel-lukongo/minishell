#ifndef MINISHELL_H
# define MINISHELL_H
# define SUCCESS 0
# define ERROR -1
#include "../libft/libft.h"
#include "../ft_printf/ft_printf.h"
#include <unistd.h>
#include<stdio.h>
#include <fcntl.h>
#include <errno.h>
int				ft_echo(char **args);
t_list			*init_env(char **env);
void			pipex(char **av, int pid1, int pid2, t_list *path);
void			redirection(int fd, char **av);
void			ft_exec(char **av, int pid1);
void			ft_close(int *fd);
char			*send_path(char *commande);
char			*my_find(t_list *lst, char *name);
void			ft_cd(char *path, t_list *env);
void			set_env(t_list *env, char *name, char *result);
void			ft_env(t_list *env);
void 			ft_unset(t_list *env, char *name);

typedef struct	m_list
{
	char *my_path;
	char **tab_path;
	char **valid_path;
}my_list;

my_list g_data;

#endif