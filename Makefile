# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/14 08:49:38 by fefa              #+#    #+#              #
#    Updated: 2025/03/14 09:08:43 by fefa             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =		minishell
CC = 		cc
CFLAGS =	-Wall -Wextra -Werror -g
HDRS :=		src/minishell.h
SRCS =		src/main.c

OBJS =		$(SRCS:.c=.o)

RM =		rm -rf

${NAME}:$(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

all: ${NAME}

clean:
	${RM} ${OBJS}

fclean: clean
		${RM} ${NAME}
		
re: fclean all

$(OBJS): $(HDRS)