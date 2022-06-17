# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alukongo <alukongo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/29 17:14:48 by alukongo          #+#    #+#              #
#    Updated: 2022/01/01 19:33:29 by alukongo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc

RM = rm -rf

NAME = libftprintf.a

MANDATORY = ft_printf.c\
			ft_printf_utils.c\
			print_hex.c\
			print_unsigned_int.c\
			print_ptr.c\

MANDATORY_SRCS	=	${MANDATORY}

BONUS_SRCS	=	${BONUS}

MANDATORY_OBJS	= ${MANDATORY_SRCS:.c=.o}

CFLAGS	= -g -Wall -Wextra -Werror

.c.o:
		${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

${NAME}: ${MANDATORY_OBJS}
			ar rc ${NAME} ${MANDATORY_OBJS}
			ranlib ${NAME}

all: ${NAME}

clean:
	${RM} ${MANDATORY_OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re