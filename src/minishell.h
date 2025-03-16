/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:09:23 by fefa              #+#    #+#             */
/*   Updated: 2025/03/16 19:04:58 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h> // readline()
# include <readline/history.h> // add_history()
# include <stddef.h>
# include <stdio.h> // perror()
# include <string.h>
# include <stdlib.h> // exit() - free()
# include <unistd.h> // write() - close()

# include <stdbool.h>
# include <fcntl.h> //open() - close()
# include <sys/types.h>
# include <sys/wait.h> // waitpid()
# include <errno.h>
# include "../libft/libft.h"

typedef enum e_type_token
{
	CMD = 1,
	ARG,
	TRUNC,
	APPEND,
	HEREDOC,
	INPUT,
	PIPE,
	END
}	t_type;

typedef struct s_cmd t_cmd;
typedef struct s_token t_token;

typedef struct s_token
{
	char	*str;
	t_type	type;
	t_token	*next;
}	t_token;

typedef	struct	s_cmd
{
	char	*cmd;
	char	**words;
	t_token	*tokens;
	t_cmd	*next;
}	t_cmd;

typedef	struct	s_mini
{
	int		fdin;
	int		fdout;
	char	**env;
	t_cmd	*cmd; //list of commands, doesnt make sense
}	t_mini;

//util_split.c
char	**ft_split_special(const char *s, char *c);

//parse.c
bool	is_open_quotes(char *line);

//token.c
void create_tokens(t_cmd *cmd);

#endif