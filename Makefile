CC = cc

RM = rm -rf

NAME = echo

MANDATORY = src/echo.c\

MANDATORY_SRCS	=	${MANDATORY}

BONUS_SRCS	=	${BONUS}

MANDATORY_OBJS	= ${MANDATORY_SRCS:.c=.o}

CFLAGS	= -g -Wall -Werror -Wextra

.c.o:
		${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

${NAME}: ${MANDATORY_OBJS}
		make -C ft_printf
		make -C libft
		${CC} ${CFLAGS} ${MANDATORY_SRCS} libft/libft.a ft_printf/libftprintf.a -o ${NAME}

all: ${NAME}

clean:
	${RM} ${MANDATORY_OBJS}

fclean: clean
	make fclean -C ft_printf
	make fclean -C libft
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re