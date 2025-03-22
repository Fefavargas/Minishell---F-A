# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/14 08:49:38 by fefa              #+#    #+#              #
#    Updated: 2025/03/21 11:06:29 by fefa             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =		minishell
CC = 		cc
CFLAGS =	-Wall -Wextra -Werror -g -Iincludes/
RLFLAGS = 	-lreadline
HDRS =		includes/minishell.h
LIBFT =		libft/libft.a
SRCS =		src/env_copy.c src/env_ft.c src/inicialize.c src/main.c src/mini.c src/parse.c \
			src/pipe.c src/redirect.c src/token.c \
			src/util_free.c src/util_split.c \
			src/builtins/pwd.c src/builtins/export.c src/builtins/env.c src/builtins/exit.c\
			src/builtins/unset.c src/builtins/echo.c src/builtins/builtin.c

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