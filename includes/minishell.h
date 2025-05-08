/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:09:23 by fefa              #+#    #+#             */
/*   Updated: 2025/05/08 09:24:04 by albermud         ###   ########.fr       */
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
# include <signal.h> //signal
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
	FILENAME,
	DELIMITER,
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

typedef struct s_sig
{
	int		sigint;
	int		sigquit;
	int		sigchld;
	int		sigexit;
}	t_sig;

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
	int				stdin;
	int				stdout;
	int				fdin;
	int				fdout;
	int				pipin;
	int				pipout;
	char			**arr_env;
	t_env			*env;
	t_env			*secret;
	t_cmd			*cmd;
	bool			execution;
	bool			exit;
	unsigned char	exit_code;
}	t_mini;

extern t_sig	g_sig;

//builtin
bool	exec_builtin(t_mini *shell, t_exec_cmd *cmd);
bool	is_builtin(char *cmd);
bool	ft_cd(t_mini *shell, char **args);
bool	ft_echo(char **args);
bool	ft_pwd(void);
bool	ft_env(t_env *env);
int		ft_unset(t_env **env, char *args[]);
bool	ft_exit(t_mini *shell, char **args);
bool	ft_export(char *args[], t_env *env, t_env *secret);

//execution.c
int		execute(t_mini *shell, t_exec_cmd *cmd);

//env_copy.c
void	ft_cpy_env(t_env **env, char **env_arr_oficial);
t_env	*get_env(t_env	*env, char *key);
void	ft_cpy_arr_env(char ***env_arr, char **env_arr_oficial);
char	*update_node(t_env *env, char *new_value);

//env_export_print.c
bool	print_export_sort(t_env *secret);
void	print_export_env_node(t_env *node);
bool	print_export_invalid_env_node(char *arg);

//env_ft.c
void	create_node_env(t_env	**node, char *str);
void	assign_env_node(t_env **new, t_env *secret, \
							char *str, bool print_error);
void	add_secret_env_node(t_env **secret, char *str);
bool	is_valid_env_node(t_env node);
void	add_env_end(t_env **env, t_env *new);

// expand_var.c
char	*expand_variable(char *str, t_mini *mini);

//heredoc.c
bool	heredoc(t_mini *shell, t_token *token);

//free.c
void	free_shell(t_mini *shell);
void	free_exec_cmd(t_exec_cmd *exec);
void	free_env(t_env *env);

//initialize.c
void	init(t_mini *shell, char **env);
void	create_cmd(char **input, t_mini *shell);
void	create_exec_cmd(t_exec_cmd *exec, t_token *token, t_mini *shell);

//mini.c
void	minishell(t_mini *shell);

//parse.c
bool	is_open_quotes(char *line);
bool	is_blanked(char *str);
bool	add_space_after(char **s, char *delimiters);
bool	add_space_before(char **str, char *delimiters);

//pipe.c
t_token	*find_next_pipe(t_token *token);
int		ft_pipe(t_mini *shell);

//redirect
bool	is_redirect_type(t_type type);
bool	redir(t_mini *shell, t_token *token_redir);
void	get_next_redir(t_token **prev, t_token *token_cmd);
//void	get_prev_redir(t_token **prev, t_token *token_cmd);

//reset.c
void	ft_close(int fd);
void	reset_loop(t_mini *shell);
void	reset_fds(t_mini *shell, bool close);
void	reset_cmd(t_mini *shell);

//signal.c
void	init_signal(void);
void	signal_int(int sig);
void	signal_quit(int sig);

//token.c
void	create_tokens(t_cmd *cmd, t_mini *shell);

// token_util.c
char	*remove_quotes(char *str);
void	type_tokens(t_token **tokens);

//util_split.c
char	**ft_split_special(const char *s, char *c);
bool	is_delimiter(char c, const char *delimiters);

//util_free.c
void	free_array(char **array);
void	free_node(t_env *env);

//util.c
void	joint_into_array_arg(char ***array, t_token *token, t_mini *shell);
void	join_into_str(char **str, char **array, char *delimitador);
void	ft_join_free(char **s1, char *s2);

#endif