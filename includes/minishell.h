/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:09:23 by fefa              #+#    #+#             */
/*   Updated: 2025/05/24 14:45:39 by fefa             ###   ########.fr       */
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
# include <termios.h>
# include <limits.h> //INT_MAX
# include "../libft/libft.h"

# define BUFFER 1024

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

typedef struct s_sig		t_sig;
typedef struct s_env		t_env;
typedef struct s_cmd		t_cmd;
typedef struct s_token		t_token;
typedef struct s_exec_cmd	t_exec_cmd;

typedef struct s_sig
{
	int			sigint;
	int			sigchld;
	int			sigexit;
}	t_sig;

typedef struct s_exec_cmd
{
	char		*cmd;
	char		**args;
	char		*str;
	int			fdin;
	int			fdout;
	bool		execution;
	t_exec_cmd	*next;
}	t_exec_cmd;

typedef struct s_env
{
	char		*key;
	char		*value;
	t_env		*next;
}	t_env;

typedef struct s_token
{
	char		*str;
	t_type		type;
	t_token		*next;
	t_token		*prev;
}	t_token;

typedef struct s_cmd
{
	char		*cmd;
	char		**words;
	int			**fdpipe;
	size_t		n_pipes;
	size_t		n_binary;
	int			*arr_pid;
	t_token		*tokens;
	t_exec_cmd	*execcmd;
	t_cmd		*next;
	bool		has_redir_error;
}	t_cmd;

typedef struct s_mini
{
	int			stdin;
	int			stdout;
	char		**arr_env;
	t_env		*env;
	t_env		*secret;
	t_cmd		*cmd;
	int			exit_code;
}	t_mini;

extern t_sig	g_sig;

//BUILTINS

bool			exec_builtin(t_mini *shell, t_exec_cmd *cmd);
bool			is_builtin(char *cmd);
bool			ft_cd(t_mini *shell, char **args);
bool			ft_echo(char **args);
bool			ft_pwd(void);
bool			ft_env(t_env *env);
bool			ft_unset(t_env **env, char *args[]);
bool			ft_exit(t_mini *shell, char **args);
bool			ft_export(char *args[], t_env *env, t_env *secret, \
							char ***env_arr);
t_env			*get_env(t_env	*env, char *key);
char			*get_path_bin(t_env *env, char *cmd);
void			assign_env_array(char ***env_arr, char *new);

//EXECUTION

//execution.c
void			execute(t_mini *shell, t_cmd *cmd);

//fdfile.c
void			ft_close(int fd);
bool			create_tmp_file(int *fd);
void			duplicate_fd(t_exec_cmd *exec);
void			update_fdin_fdout(t_exec_cmd **exec, t_cmd *cmd,
					int i, int n_pipes);
void			create_array_pids(t_cmd *cmd);

//heredoc.c
bool			heredoc(t_mini *shell, t_token *token, t_exec_cmd *exec);

//mini.c
void			minishell(t_mini *shell);

//pipes.c
void			close_pipes(t_cmd	*cmd);
void			create_pipes(t_cmd *cmd);
bool			find_pipe_sequence(t_cmd *cmd);

//redirect_util.c
int				is_redirect(const char *str);
bool			is_redirect_type(t_type type);
t_type			type_redirect(char *str);
bool			ensure_directory_exists(t_env *env, const char *path);

//redirect.c
bool			redir(t_mini *shell, t_exec_cmd *cmd, t_token *token_redir);

//signal.c
void			heredoc_sigint_handler(int sig);
void			init_signal(t_mini *shell);
void			signal_int(int sig);
void			signal_chld(void);

//token.c
int				count_link_list(t_token *token);
void			create_tokens(t_cmd *cmd, t_mini *shell);

//wait_fork.c
int				error_message(char *path);
void			wait_fork(t_mini *shell, t_cmd *cmd);

//OTHERS

//error.c
int				print_error(char *str, int num);
int				error_msg(char *str1, char *str2, char *str3, int ret);
void			print_eof_warning(char *delimiter);

//reset.c
void			free_shell(t_mini *shell);
void			reset_loop(t_mini *shell, char **input);
void			reset_cmd_list(t_mini *shell);

//util_free.c
void			free_node(t_env *env);
void			free_env(t_env *env);
void			free_exec_cmd(t_exec_cmd *exec);
char			**free_array(char **array);
void			free_array_int(int **array, size_t n);

//PARSE

//env_copy.c
void			ft_cpy_arr_env(char ***env_arr, char **env_arr_oficial);
void			ft_cpy_env(t_env **env, char **env_arr_oficial);
bool			update_node(t_env **env, char *new_value);
bool			update_node_key(t_env *env, char *key, char *path);

//env_util.c
void			create_node_env(t_env	**node, char *str);
bool			assign_env_node(t_env *secret, char *str, bool print_error);
void			add_env_end(t_env **env, t_env *new);

//expand_var.c
void			expand_variable(char **str, t_mini *shell);

//ft_join_special.c
void			ft_join_free(char **s1, char *s2);
int				joint_into_array_arg(char ***array, t_token *token);
void			join_into_str(char **str, char **array, char *delimitador);
char			*strjoin_three(const char *s1, const char *s2, const char *s3);

//ft_split_special_util.c
size_t			count_words(char const *s, char *delimiters);

//util_split.c
char			**ft_split_special(const char *s, char *c);

//initialize.c
bool			create_cmd_list(char *input, t_mini *shell);
void			init(t_mini *shell, char **env);

//parse_space.c
void			add_space_before(char **str, char *delimiters);
void			add_space_after(char **s, char *delimiters);
bool			add_string_middle(char **s, char *add, size_t pos);

//parse.c
char			*remove_quotes(char *str);
bool			parse(char **input, t_mini *shell);

// token_type.c
void			get_next_type(t_token	**token, t_type type);
bool			find_prev_type(t_token *token, t_type type);
void			type_tokens(t_token **tokens);

//util.c
bool			is_delimiter(char c, const char *delimiters);
bool			is_blanked(char *str);
const char		*get_add_str(const char *s);
char			*substr(const char *s, size_t start, size_t len);

#endif