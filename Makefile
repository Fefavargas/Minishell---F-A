# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/14 08:49:38 by fefa              #+#    #+#              #
#    Updated: 2025/05/18 20:35:32 by fefa             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME =		minishell
CC = 		cc
CFLAGS =	-Wall -Wextra -Werror -g -Iincludes/
RLFLAGS = 	-lreadline
HDRS =		includes/minishell.h
LIBFT =		libft/libft.a
SRCS =		src/env_copy.c src/env_util.c src/expand_var.c src/expand_var_utils.c \
			src/fdfile.c src/heredoc.c src/heredoc_utils.c src/initialize.c src/main.c \
			src/reset.c src/signal.c src/token_type.c \
			src/util_free1.c src/util_free2.c src/util_join.c src/util.c \
			src/builtins/pwd.c src/builtins/export.c src/builtins/env.c src/builtins/exit.c \
			src/builtins/unset.c src/builtins/echo.c src/builtins/cd.c src/builtins/builtin.c src/builtins/util_builtin.c \
			src/execution/execution.c src/execution/mini.c src/execution/pipe.c src/execution/redirect_util.c src/execution/redirect.c src/execution/token.c src/execution/wait_fork.c \
			src/parse/ft_split_special_util.c src/parse/ft_split_special.c src/parse/parse.c src/parse/parse_space.c

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