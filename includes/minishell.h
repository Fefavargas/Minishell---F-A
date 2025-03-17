/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:09:23 by fefa              #+#    #+#             */
/*   Updated: 2025/03/17 06:11:23 by fefa             ###   ########.fr       */
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

# define SUCCESS 0
# define ERROR 1
# define BUFFER 1024

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

typedef struct s_env t_env;
typedef struct s_cmd t_cmd;
typedef struct s_token t_token;

typedef struct	s_env
{
	char	*key;
	char	*value;
	t_env	*next;
}	t_env;

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
	t_env	*env;
	t_cmd	*cmd; //list of commands, doesnt make sense
}	t_mini;

//builtin
bool	ft_echo(char **args);
bool	ft_pwd();
bool	ft_env(t_mini *shell);

//inicialize.c
void	inic(t_mini *shell, char **env);
void	create_cmd(char *input, t_mini *shell);

//util_split.c
char	**ft_split_special(const char *s, char *c);

//util_free.c
void	free_array(char **array);

//parse.c
bool	is_open_quotes(char *line);

//token.c
void	create_tokens(t_cmd *cmd);

#endif