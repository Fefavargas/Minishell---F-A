# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/14 08:49:38 by fefa              #+#    #+#              #
#    Updated: 2025/03/16 08:09:09 by fefa             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =		minishell
CC = 		cc
CFLAGS =	-Wall -Wextra -Werror -g
RLFLAGS = 	-lreadline
HDRS :=		src/minishell.h
LIBFT =		libft/libft.a
SRCS =		src/main.c

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