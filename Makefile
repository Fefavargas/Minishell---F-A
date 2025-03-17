# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/14 08:49:38 by fefa              #+#    #+#              #
#    Updated: 2025/03/17 06:44:30 by fefa             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =		minishell
CC = 		cc
CFLAGS =	-Wall -Wextra -Werror -g -Iincludes/
RLFLAGS = 	-lreadline
HDRS =		includes/minishell.h
LIBFT =		libft/libft.a
SRCS =		src/main.c src/parse.c src/util_free.c src/util_split.c \
			src/token.c src/env_copy.c src/inicialize.c src/builtins/echo.c \
			src/builtins/pwd.c src/builtins/env.c 

OBJS =		$(SRCS:.c=.o)

RM =		rm -rf

${NAME}:$(OBJS) $(LIBFT)
		$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(RLFLAGS) -o $(NAME)

all: ${NAME}

clean:
	${RM} ${OBJS}
	make clean -C ./libft

fclean: clean
		${RM} ${NAME}
		
re: fclean all

$(OBJS): $(HDRS)