/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:09:23 by fefa              #+#    #+#             */
/*   Updated: 2025/04/05 18:37:01 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h> // readline()
# include <readline/history.h> // add_history()
# include <stddef.h> //size_t
# include <stdio.h> // perror()
# include <stdlib.h> // exit() - free()
# include <unistd.h> // write() - close()
# include <stdbool.h> //bool
# include <fcntl.h> //open() - close()
# include <sys/wait.h> // waitpid()
# include <sys/stat.h> //stat 
# include <sys/types.h> //pid_t
# include <string.h>
# include <errno.h> //strerror
# include "../libft/libft.h"

# define SUCCESS 1
# define ERROR 0
# define TRUE 1
# define FALSE 0
# define BUFFER 1024
# define IS_DIRECTORY 126
# define UNKNOWN_COMMAND 127

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
	P_DEFAULT = -1,
	P_CHILD,
	P_PARENT
}	t_type_pipe;

typedef struct s_env	t_env;
typedef struct s_cmd	t_cmd;
typedef struct s_token	t_token;

typedef struct s_exec_cmd
{
	char	*cmd;
	char	**args;
	char	*str;
}	t_exec_cmd;

typedef struct s_env
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

typedef struct s_cmd
{
	char	*cmd;
	char	**words;
	t_token	*tokens;
	t_cmd	*next;
}	t_cmd;

typedef struct s_mini
{
	int		fdin;
	int		fdout;
	int		pid;
	int		pipin;
	int		pipout;
	char	**arr_env;
	t_env	*env;
	t_env	*secret;
	t_cmd	*cmd; //list of commands, doesnt make sense
	bool	exit;
}	t_mini;

//builtin
bool	exec_builtin(t_mini *shell, t_exec_cmd *cmd);
bool	is_builtin(char *cmd);
bool	ft_cd(t_env *env, char *arg);
bool	ft_echo(char **args);
bool	ft_pwd(void);
bool	ft_env(t_env *env);
bool	ft_unset(t_env *env, char *unset);
bool	ft_exit(t_mini *shell, char **args);
bool	ft_export(char *arg, t_env *env, t_env *secret);

//execution.c
int		execute(t_mini *shell, t_exec_cmd *cmd);

//env_copy.c
void	ft_cpy_env(t_env **env, char **env_arr_oficial);
t_env	*get_env(t_env	*env, char *key);
void	ft_cpy_arr_env(char ***env_arr, char **env_arr_oficial);

//env_ft.c
void	create_node_env(t_env	**node, char *str);
bool	is_valid_env_node(t_env node);
void	add_env_end(t_env **env, t_env *new);
char	*update_node(t_env *env, char *new_value);

//free.c
void	free_shell(t_mini *shell);
void	free_exec_cmd(t_exec_cmd *exec);
void	free_env(t_env *env);

//initialize.c
void	init(t_mini *shell, char **env);
void	create_cmd(char *input, t_mini *shell);
void	create_exec_cmd(t_exec_cmd *exec, t_token *token);

//mini.c
void	minishell(t_mini *shell);

//parse.c
bool	is_open_quotes(char *line);

//pipe.c
int		pipex(t_mini *shell);

//redirect
void	redir(t_mini *shell, t_token *token);

//reset.c
void	ft_close(int fd);
void	reset_loop(t_mini *shell);
void	reset_fds(t_mini *shell, bool close);
void	reset_cmd(t_mini *shell);

//token.c
void	create_tokens(t_cmd *cmd);

// token_util.c
char	*remove_quotes(char *str);

//util_split.c
char	**ft_split_special(const char *s, char *c);

//util_free.c
void	free_array(char **array);
void	free_node(t_env *env);

//util.c
void	joint_into_array_arg(char ***array, t_token *token);
void	join_into_str(char **str, char **array, char *delimitador);

#endif