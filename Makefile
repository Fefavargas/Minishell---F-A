# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: albbermu <albbermu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/14 08:49:38 by fefa              #+#    #+#              #
#    Updated: 2025/04/11 14:05:56 by albbermu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =		minishell
CC = 		cc
CFLAGS =	-Wall -Wextra -Werror -g -Iincludes/
RLFLAGS = 	-lreadline
HDRS =		includes/minishell.h
LIBFT =		libft/libft.a
SRCS =		src/env_copy.c src/env_ft.c src/execution.c src/expand_var.c src/initialize.c src/main.c src/mini.c src/parse.c \
			src/pipe.c src/redirect.c src/reset.c src/token.c src/token_utils.c \
			src/util_free1.c src/util_free2.c src/util_split.c src/util.c \
			src/builtins/pwd.c src/builtins/export.c src/builtins/env.c src/builtins/exit.c\
			src/builtins/unset.c src/builtins/echo.c src/builtins/cd.c src/builtins/builtin.c

OBJDIR = objs
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))

RM =		rm -rf

${NAME}: $(OBJS) $(LIBFT)
		$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(RLFLAGS) -o $(NAME)

$(OBJDIR)/%.o: %.c $(HDRS)
		mkdir -p $(dir $@)  # Ensure the directory exists
		$(CC) $(CFLAGS) -c $< -o $@

all: ${NAME}

clean:
	${RM} ${OBJS}
	make clean -C ./libft

fclean: clean
		${RM} ${NAME}
		
re: fclean all

$(OBJS): $(HDRS)