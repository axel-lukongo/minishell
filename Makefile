# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: denissereno <denissereno@student.42.fr>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/09 13:04:45 by darian            #+#    #+#              #
#    Updated: 2022/09/13 11:25:22 by denissereno      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

BLU			= \033[0;34m
GRN			= \033[0;32m
RED			= \033[0;31m
RST			= \033[0m
END			= \e[0m

SRCS		= 	srcs/main.c \
				srcs/env.c \
				srcs/utils.c \
				srcs/utils_2.c \
				srcs/utils_3.c \
				srcs/utils_4.c \
				srcs/ustack.c \
				srcs/ustack_2.c \
\
				srcs/execute/pipex.c \
				srcs/execute/binary_op.c \
				srcs/execute/exec.c \
				srcs/execute/exec_builtin.c \
				srcs/execute/exec_utils.c \
				srcs/execute/error.c \
				srcs/execute/pipe.c \
				srcs/execute/redirection/redir1.c \
				srcs/execute/redirection/redir2.c \
				srcs/execute/redirection/redir3.c \
\
				srcs/lexer/lexer.c \
				srcs/lexer/lexer_2.c \
				srcs/lexer/lexer_3.c \
				srcs/lexer/lexer_4.c \
				srcs/lexer/lexer_5.c \
				srcs/lexer/lexer_6.c \
				srcs/lexer/lexer_utils_1.c \
				srcs/lexer/lexer_utils_2.c \
				srcs/lexer/lexer_utils_3.c \
\
				srcs/btree/print_tree.c \
				srcs/btree/tree.c \
\
				srcs/parsing/parser.c \
				srcs/parsing/here_doc.c \
				srcs/parsing/parser_2.c \
				srcs/parsing/parser_utils.c \
\
				srcs/expander/env.c \
				srcs/expander/env_2.c \
				srcs/expander/env_3.c \
				srcs/expander/match.c \
				srcs/expander/back.c \
\
				srcs/builtin/export.c \
				srcs/builtin/cd.c\
				srcs/builtin/builtin_utils.c\
				srcs/builtin/echo.c\
				srcs/builtin/builtin_utils2.c\
				srcs/builtin/builtin_utils3.c\
				srcs/builtin/ft_unset.c\


NAME		= minishell
OBJS_DIR	= objs/
OBJS		= $(SRCS:.c=.o)
OBJECTS_PREFIXED = $(addprefix $(OBJS_DIR), $(OBJS))
CC			= gcc
CC_FLAGS	= -Wall -Werror -Wextra -g3
TFLAGS		= -I -L -Lincludes -lreadline libft/libft.a 

$(OBJS_DIR)%.o : %.c includes/minishell.h
	@mkdir -p $(OBJS_DIR)
	@mkdir -p $(OBJS_DIR)srcs
	@mkdir -p $(OBJS_DIR)srcs/btree
	@mkdir -p $(OBJS_DIR)srcs/expander
	@mkdir -p $(OBJS_DIR)srcs/parsing
	@mkdir -p $(OBJS_DIR)srcs/lexer
	@mkdir -p $(OBJS_DIR)srcs/execute
	@mkdir -p $(OBJS_DIR)srcs/builtin
	@mkdir -p $(OBJS_DIR)srcs/execute/redirection
	@$(CC) $(CC_FLAGS) -c $< -o $@
	@printf	"\033[2K\r${BLU}[BUILD - $(NAME)]${RST} '$<' $(END)"

$(NAME): $(OBJECTS_PREFIXED)
	make bonus -C libft
	@$(CC) -o $(NAME) $(OBJECTS_PREFIXED) $(CC_FLAGS) $(TFLAGS)
	@printf "\033[2K\r\033[0;32m[END]\033[0m $(NAME)$(END)\n"

all: $(NAME)

clean:
	make clean -C libft
	@rm -rf $(OBJS_DIR)
	@echo "${GRN}[CLEAN]${RST} done"

fclean: clean
	make fclean -C libft
	@rm -f $(NAME)
	@echo "${GRN}[FCLEAN]${RST} done"

re: fclean all

.PHONY:		all clean fclean re
