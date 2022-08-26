CC = cc

RM = rm -rf

NAME = minishell

MANDATORY = pipex.c\
			child.c\
			error.c\
			files.c\
			free.c\
			here_doc.c\
			gnl/get_next_line.c\
			gnl/get_next_line_utils.c\
			functions/ft_split.c\
			functions/ft_strdup.c\
			functions/ft_strjoin.c\
			functions/ft_strncmp.c\
			

MANDATORY_SRCS	=	${MANDATORY}

BONUS_SRCS	=	${BONUS}

MANDATORY_OBJS	= ${MANDATORY_SRCS:.c=.o}

CFLAGS	= -g -Wall -Werror -Wextra

.c.o:
		${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

${NAME}: ${MANDATORY_OBJS}
		${CC} ${CFLAGS} ${MANDATORY_SRCS} -o ${NAME}
		@echo -e "$(GREEN)$(NAME) created!$(DEFAULT)"

all: ${NAME}

clean:
	${RM} ${MANDATORY_OBJS}
	@echo -e "$(YELLOW)objet deleted!$(DEFAULT)"

fclean: clean
	${RM} ${NAME}
	@echo -e "$(RED)all deleted!$(DEFAULT)"

re: fclean all

.PHONY: all clean fclean re

YELLOW = \033[1;33m
RED = \033[1;31m
GREEN = \033[1;32m
DEFAULT = \033[0m
