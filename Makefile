# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/14 08:49:38 by fefa              #+#    #+#              #
#    Updated: 2025/05/18 21:29:01 by fefa             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME =		minishell
CC = 		cc
CFLAGS =	-Wall -Wextra -Werror -g -Iincludes/
RLFLAGS = 	-lreadline
HDRS =		includes/minishell.h
LIBFT =		libft/libft.a
SRCS =		src/main.c \
			src/builtins/pwd.c src/builtins/export.c src/builtins/env.c src/builtins/exit.c \
			src/builtins/unset.c src/builtins/echo.c src/builtins/cd.c src/builtins/builtin.c src/builtins/util_builtin.c \
			src/execution/execution.c  src/execution/heredoc.c src/execution/fdfile.c src/execution/mini.c \
			src/execution/pipe.c src/execution/redirect_util.c src/execution/redirect.c src/execution/signal.c src/execution/token.c src/execution/wait_fork.c \
			src/others/error.c src/others/print_tokens.c src/others/reset.c src/others/util_free.c \
			src/parse/env_copy.c src/parse/env_util.c src/parse/expand_var.c  src/parse/ft_join_special.c src/parse/ft_split_special_util.c \
			src/parse/ft_split_special.c src/parse/initialize.c src/parse/parse.c src/parse/parse_space.c src/parse/token_type.c src/parse/util.c

OBJDIR = objs
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))

VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose

RM =		rm -rf

${NAME}: $(OBJS) $(LIBFT)
		$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(RLFLAGS) -o $(NAME)

$(OBJDIR)/%.o: %.c $(HDRS)
		mkdir -p $(dir $@)  # Ensure the directory exists
		$(CC) $(CFLAGS) -c $< -o $@

all: ${NAME}

clean:
	${RM} $(OBJS)
	${RM} -d $(OBJDIR)/src/builtins 2>/dev/null || true
	${RM} -d $(OBJDIR)/src 2>/dev/null || true
	${RM} -d $(OBJDIR) 2>/dev/null || true
	make clean -C ./libft

fclean: clean
		${RM} ${NAME}
		
re: fclean all

valgrind-supp:
	@echo "creating valgrind suppression file"
	@echo "{" > readline.supp
	@echo "   Readline" >> readline.supp
	@echo "   Memcheck:Leak" >> readline.supp
	@echo "   match-leak-kinds: reachable" >> readline.supp
	@echo "   ..." >> readline.supp
	@echo "   fun:readline" >> readline.supp
	@echo "}" >> readline.supp
	@echo "{" >> readline.supp
	@echo "   AddHistory" >> readline.supp
	@echo "   Memcheck:Leak" >> readline.supp
	@echo "   match-leak-kinds: reachable" >> readline.supp
	@echo "   ..." >> readline.supp
	@echo "   fun:add_history" >> readline.supp
	@echo "}" >> readline.supp

valgrind: ${NAME} valgrind-supp
	$(VALGRIND) --suppressions=readline.supp ./$(NAME)

memcheck: ${NAME} valgrind-supp
	$(VALGRIND) --suppressions=readline.supp --log-file=valgrind_output.txt ./$(NAME)

.PHONY: all clean fclean re valgrind memcheck

$(OBJS): $(HDRS)