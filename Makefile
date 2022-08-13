# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/09 13:04:45 by darian            #+#    #+#              #
#    Updated: 2022/08/13 19:41:07 by alukongo         ###   ########.fr        #
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
				srcs/ustack.c \
				srcs/builtin/cd.c\
				srcs/builtin/builtin_utils.c\
				srcs/builtin/echo.c\
\
				srcs/execute/executor.c \
				srcs/execute/pipex.c \
				srcs/execute/exec_builtin.c \
\
				srcs/lexer/lexer.c \
\
				srcs/btree/print_tree.c \
				srcs/btree/tree.c \
				srcs/btree/get.c \
\
				srcs/parsing/parser.c \
\
				srcs/expander/env.c \
				srcs/expander/match.c \
				srcs/builtin/export.c \
\


NAME		= minishell
OBJS_DIR	= objs/
OBJS		= $(SRCS:.c=.o)
OBJECTS_PREFIXED = $(addprefix $(OBJS_DIR), $(OBJS))
CC			= gcc
CC_FLAGS	= -Wall -Werror -Wextra -g #-fsanitize=address
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