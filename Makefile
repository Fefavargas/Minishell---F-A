# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/14 08:49:38 by fefa              #+#    #+#              #
#    Updated: 2025/05/18 16:23:20 by fefa             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME =		minishell
CC = 		cc
CFLAGS =	-Wall -Wextra -Werror -g -Iincludes/
RLFLAGS = 	-lreadline
HDRS =		includes/minishell.h
LIBFT =		libft/libft.a
SRCS =		src/env_copy.c src/env_ft.c src/env_ft2.c src/execution.c src/execution_wait_fork.c src/expand_var.c src/expand_var_utils.c \
			src/fdfile.c src/fdfile2.c src/heredoc.c src/heredoc_utils.c src/initialize.c src/main.c src/mini.c src/mini_exec_cmds.c src/parse.c src/parse_space.c \
			src/pipe.c src/redirect.c src/redirect2.c src/reset.c src/signal.c src/token.c src/token_utils.c src/token_utils2.c \
			src/util_free1.c src/util_free2.c src/util_split.c src/util_split2.c src/util_join.c src/util.c \
			src/builtins/pwd.c src/builtins/export.c src/builtins/env.c src/builtins/exit.c\
			src/builtins/unset.c src/builtins/echo.c src/builtins/cd.c src/builtins/builtin.c

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