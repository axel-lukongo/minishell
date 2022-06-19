CC = cc

RM = rm -rf

NAME = env

MANDATORY = src/echo.c\
			src/env.c\

MANDATORY_SRCS	=	${MANDATORY}

BONUS_SRCS	=	${BONUS}

MANDATORY_OBJS	= ${MANDATORY_SRCS:.c=.o}

CFLAGS	= -g -Wall -Werror -Wextra

.c.o:
		${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

${NAME}: ${MANDATORY_OBJS}
		make -C ft_printf
		make bonus -C libft
		${CC} ${CFLAGS} ${MANDATORY_SRCS} libft/libft.a ft_printf/libftprintf.a -o ${NAME}
		@echo -e "$(GREEN)$(NAME) created!$(DEFAULT)"

all: ${NAME}

clean:
	${RM} ${MANDATORY_OBJS}
	make fclean -C ft_printf
	make fclean -C libft
	@echo -e "$(YELLOW)objet deleted!$(DEFAULT)"

fclean: clean
	make fclean -C ft_printf
	make fclean -C libft
	${RM} ${NAME}
	@echo -e "$(RED)all deleted!$(DEFAULT)"

re: fclean all

.PHONY: all clean fclean re

YELLOW = \033[1;33m
RED = \033[1;31m
GREEN = \033[1;32m
DEFAULT = \033[0m
