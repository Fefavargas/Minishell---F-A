/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:09:23 by fefa              #+#    #+#             */
/*   Updated: 2025/03/23 12:42:20 by fefa             ###   ########.fr       */
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

typedef enum e_type_pipe
{
	P_PARENT = 1,
	P_CHILD,	
}	t_type_pipe;

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
	t_token	*prev;
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
	int		pid;
	int		pipin;
	int		pipout;
	bool	exit;
}	t_mini;

//builtin
bool	ft_cd(t_env *env, char *arg);
bool	ft_echo(char **args);
bool	ft_pwd();
bool	ft_env(t_env *env);
bool	ft_unset(t_env *env, char *unset);
bool	ft_exit(t_mini *shell);
bool	ft_export(char *arg, t_env *env);

//env_copy.c
void	ft_copy_env(t_mini *shell, char **env);
t_env	*get_env(t_env	*env, char *key);

//env_ft.c
void	create_node_env(t_env	**node, char *str);
bool	is_valid_env_node(t_env node);
void	add_env_end(t_env **env, t_env *new);
char	*update_node(t_env *env, char *new_value);

//inicialize.c
void	inic(t_mini *shell, char **env);
void	create_cmd(char *input, t_mini *shell);
void	reset(t_mini *shell);

//mini.c
void	minishell(t_mini *shell);

//parse.c
bool	is_open_quotes(char *line);

//pipe.c
int		pipex(t_mini *shell);

//redirect
void	redir(t_mini *shell, t_token *token);

//token.c
void	create_tokens(t_cmd *cmd);

//util_split.c
char	**ft_split_special(const char *s, char *c);

//util_free.c
void	free_array(char **array);

#endif