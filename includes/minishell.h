/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 17:08:09 by dasereno          #+#    #+#             */
/*   Updated: 2022/08/19 14:46:53 by denissereno      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <signal.h>
# include <dirent.h>
#include <stdint.h>
#include <errno.h>

# include "ansi.h"
# include "../libft/libft.h"

// < > << >> | || && ARG CMD FILE ' " \ ENV 

# define CMD 0
# define ARG 1
# define FILE 2
# define ENV 3
# define SEMI 4
# define LBRACE 5
# define RBRACE 6
# define S_QUOTE 7
# define D_QUOTE 8
# define LESS 9
# define DLESS 10
# define GREAT 11
# define DGREAT 12
# define PIPE 13
# define WILDCARD 14
# define OR 15
# define AND 16
# define SEP 17
# define END 18
# define RESULT 19
# define WILDENV 20
# define BACKSLASH 21
# define WILDBACK 22
# define BACKENV 23
# define WILDENVBACK 24

# define DEFAULT "\001\033[0;39m\002"
# define GRAY "\001\033[1;90m\002"
// # define RED "\001\033[1;91m\002"
# define GREEN "\001\033[1;92m\002"
# define YELLOW "\001\033[1;93m\002"
# define BLUE "\001\033[1;94m\002"
# define MAGENTA "\001\033[1;95m\002"
# define CYAN "\001\033[1;96m\002"
# define WHITE "\001\033[0;97m\002"
# define BOLD "\033[1m"
# define RESET "\033[0m"

typedef struct s_token {
	int				type;
	char			*str;
}	t_token;

typedef struct s_lexer
{
	t_token	*llisttok;
	int		ntok;
}	t_lexer;

typedef struct s_tree
{
	int				type;
	int				id;
	int				return_val;
	char			*value;
	struct s_tree	*left;
	struct s_tree	*right;
	struct s_tree	*parent;
}	t_tree;

typedef struct	s_parsing
{
	t_token	*next_token;
	t_list	*li;
}	t_parsing;

typedef struct	s_env
{
	char	*name;
	char	*value;
}	t_env;

typedef struct	s_vector2D
{
	int	x;
	int	y;
}	t_vector2D;

typedef struct	s_exec
{
	char			*cmd;
	int				redirect_last;
}	t_exec;

typedef struct	s_exec_con
{
	t_exec	**exec;
	t_tree	*parent;
	int		nb_cmd;
	int		node_id;
	int		return_val;
	int		is_left;
}	t_exec_con;

typedef struct	s_ustack
{
	int		stack_size;
	char	**stack;
}	t_ustack;

typedef struct	s_global
{
	t_alloc		*alloc;
	t_list		*env;
	char		pwd[1024];
	char		disp_pwd[1024];
	int			last_return;
	pid_t		sh_pid;
	t_list		*lex;
	t_tree		*ast;
	char		*line;
	int			node_id;
	char		**char_env;
	t_ustack	*dir_stack;
	int			writed;
}	t_global;

typedef struct	s_lex
{
	char		*buf;
	t_list		*t_lst;
	int			buf_size;
	int			i;
	int			c;
	int			k;
	int			start;
	t_vector2D	q;
	t_vector2D	quoted;
	int			parent;
	int			backed;
	int			wildcarded;
	int			enved;
}	t_lex;

extern t_parsing *g_p;

// -> LIST
void	print_list(t_list *li);
void	print_list_env(t_list *li);
void	destroy_env_var(t_global *g, char *name);
//
// TREE
t_tree	*new_tree(t_token tok, t_global *g);
t_tree	*join_tree(t_tree *left, t_tree *right, t_token tok, t_global *g);
void	clean_tree(t_tree **tr);
void	print_tree(t_tree *tr);
int		count_tree_nodes(t_tree *tr);
//
// -> UTILS
int		ft_cmptomax(char *s1, char *s2);
void	copy_str(char dest[1024], char *src);
//
// // <====== MINISHELL ======>
//
// LEXER
t_list	*lexer(char *buffer, t_alloc **alloc);
void	add_to_list(int type, char *str, t_list **li, t_alloc **alloc);
void	add_to_list_front(int type, char *str, t_list **li, t_alloc **alloc);
void	concat_to_last(t_token	tok, t_list **lst,
	t_alloc **alloc, int change_type);
void	concat_to_last_no_space(t_token	tok, t_list **lst,
	t_alloc **alloc, int change_type);
int		ft_istoken(char c);
int		ft_istoken_no_space(char c);
int		ft_isspace(char c);
int		get_btok(t_list *li, int pos);
void	end_add_wildcarded_not_start(t_alloc **alloc, t_lex *lex);
void	end_if_enved(t_alloc **alloc, t_lex *lex);
void	end_other_option(t_alloc **alloc, t_lex *lex);
void	end_add_last(t_alloc **alloc, t_lex *lex);
int		add_not_quoted_2(char *b, t_alloc **alloc, t_lex *lex);
void	add_simple_quote(char *buffer, t_alloc **alloc, t_lex *lex);
void	add_parent(t_alloc **alloc, t_lex *lex, char *s, int type);
int		add_wildcard(char *buffer, t_alloc **alloc, t_lex *lex);
void	buffer_writing(char *buffer, t_alloc **alloc, t_lex *lex);
void	add_redir(char *s, t_alloc **alloc, t_lex *lex, int type);
void	add_binop(char *s, t_alloc **alloc, t_lex *lex, int type);
void	write_char(char *buffer, t_alloc **alloc, t_lex *lex);
void	first_wildcarded(char *buffer, t_alloc **alloc, t_lex *lex);
void	add_type(int type, t_lex *lex, t_alloc **alloc);
void	concat_other(char *buffer, t_alloc **alloc, t_lex *lex);
void	buf_to_list(char *buffer, t_alloc **alloc, t_lex *lex);
void	add_double_quote(char *buffer, t_alloc **alloc, t_lex *lex);
//
// PARSER
//
t_tree	*parse_op(t_global *g);
t_tree	*parse_redir(t_global *g);
t_tree	*parse_word(t_global *g);
t_tree	*parsing(t_list *tli, t_global *g);
t_tree	*create_token_node(int type, t_global *g);
t_tree	*left_brace(t_tree *tr, t_global *g);
t_tree	*create_exp_token_node(int type, char *str, t_tree **a, t_global *g);
//
// ENV
int		is_shell_char_var_allowed(char c);
t_list	*init_env(char **env, t_alloc *alloc, int i);
t_env	*get_node_by_name(t_list *env, char *name);
char	*get_value_by_name(t_list *env, char *name);
void	change_value_by_name(t_global *g, char *name, char *value);
int		is_var_env_exist(t_list	*env, char *name);
//
// EXPANDER
void	expander(t_tree *ast, t_global *g);
char	*wildcard(char *str, t_alloc *alloc);
int		is_char(char *str, char c);
int		when_is_char(char *str, char c);
int		count_char(char *str, int ch);
void	print_tree_command_line(t_tree	*tr);
char	*backslash(char *str, t_global *g);
char	*get_var_name(char *str, int i);
void	double_dollar(char **result, t_global *g, t_vector2D *it, char *str);
void	dollar_question(char **result, t_global *g, t_vector2D *it, char *str);
//
// EXEC
void	pipex(int n, t_exec **cmd, t_list *env, t_global *g);
void	execute(t_global *g);
int 	is_builtin(char *str);
void    execute_builtin(t_global *g, char **cmd);
char	*delete_quote(char *str, t_alloc **alloc);
char	**convert_tree_to_cmd(t_tree	*tr, t_global *g);
int		is_directory(char *str);

//BUILTIN
int		is_builtin(char *str);
void	my_cd(t_global *g, char **cmd);
void	cd_dash_or_nothing(t_global *g, char **cmd);
void	my_cd2(t_global *g);
void	cd_error_msg(t_global *g, char **cmd);
t_list	*ft_cpy_env(t_list *dest, t_list *src, int src_size);
int		cmp(void *content, void *content_ref);
void	ft_list_sort(t_list **begin_list, int (*cmp)());
void	my_export(t_global *g, char **cmd);
int		count_char(char *str, int ch);
char	*del_last_path(t_global *g, char *path);
int		dir_change_stack(char *str);
int		is_allowed_var(char *var);
void	ft_echo(char **args);

//
// USTACK
void		print_ustack(t_ustack *stack);
int			is_in_ustack(t_ustack *stack, char *value);
void		pop_ustack(t_ustack *stack, char *value, int index, int type);
void		push_ustack(t_ustack *stack, char *value);
void		clear_ustack(t_ustack *stack);
t_ustack	*init_ustack(int size, t_global *g);
char		*get_value_ustack(t_ustack *st, int index);

typedef struct Trunk
{
    struct Trunk *prev;
    char *str;
}	Trunk;
void printTree(t_tree* root, Trunk *prev, int isLeft);

t_exec_con	*get_exec_container(t_global *g, t_tree *tr);
void	delete_tree(t_tree* node, int id, t_tree **root);
t_tree	*get_first_exec_node(t_tree *tr);
int		count_executable_nodes(t_tree	*tr);
#endif