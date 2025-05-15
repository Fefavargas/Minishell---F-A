/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albbermu <albbermu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:09:23 by fefa              #+#    #+#             */
/*   Updated: 2025/05/15 15:50:59 by albbermu         ###   ########.fr       */
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

typedef struct s_env		t_env;
typedef struct s_cmd		t_cmd;
typedef struct s_token		t_token;
typedef struct s_exec_cmd	t_exec_cmd;

typedef struct s_sig
{
	int		sigint;
	int		sigquit;
	int		sigchld;
	int		sigexit;
	int		heredoc;
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
	char	*key;
	char	*value;
	t_env	*next;
}	t_env;

typedef struct s_token
{
	char	*str; //DELETE LATER
	t_type	type;
	t_token	*next;
	t_token	*prev;
}	t_token;

typedef struct s_cmd
{
	char		*cmd;
	char		**words;
	int			**fdpipe;
	size_t		n_pipes;
	int			*arr_pid;
	t_token		*tokens;
	t_exec_cmd	*execcmd;
	t_cmd		*next;
	bool		has_redir_error;
}	t_cmd;

typedef struct s_mini
{
	int		stdin;
	int		stdout;
	char	**arr_env;
	t_env	*env;
	t_env	*secret;
	t_cmd	*cmd;
	bool	exit;
	int		exit_code;
}	t_mini;

extern t_sig	g_sig;

//builtin
bool	exec_builtin(t_mini *shell, t_exec_cmd *cmd);
bool	is_builtin(char *cmd);
bool	ft_cd(t_mini *shell, char **args);
bool	ft_echo(char **args);
bool	ft_pwd(void);
bool	ft_env(t_env *env);
bool	ft_unset(t_env **env, char *args[]);
bool	ft_exit(t_mini *shell, char **args);
bool	ft_export(char *args[], t_env *env, t_env *secret);

//env_copy.c
void	ft_cpy_env(t_env **env, char **env_arr_oficial);
t_env	*get_env(t_env	*env, char *key);
void	ft_cpy_arr_env(char ***env_arr, char **env_arr_oficial);
bool	update_node(t_env *env, char *new_value);
bool	update_node_key(t_env *env, char *key, char *path);

//env_export_print.c
bool	print_export_sort(t_env *secret);
void	print_export_env_node(t_env *node);

//env_ft.c
void	create_node_env(t_env	**node, char *str);
void	assign_env_node(t_env *secret, char *str, bool print_error);
void	add_secret_env_node(t_env **secret, char *str);
bool	is_valid_env_node(t_env node);
void	add_env_end(t_env **env, t_env *new);

//execution.c
char	*get_path_bin(t_env *env, char *cmd);
void	execute(t_mini *shell, t_cmd *cmd);
int		error_message(char *path);

//expand_var.c
void	expand_variable(char **str, t_mini *shell);

//file.c
bool	find_ampersand(char *input);
void	close_cmd(t_cmd	*cmd);
void	prepare_parent(int *pid, t_exec_cmd *exec);
void	create_array_pids(t_cmd *cmd);
void	prepare_chld(t_mini *shell, t_exec_cmd *exec, t_cmd *cmd);

//heredoc.c
int		heredoc(t_mini *shell, t_token *token);

//heredoc_utils.c
bool	create_tmp_file(int *fd);
void	print_eof_warning(char *delimiter);
void	process_heredoc_line(int fd, char *str, t_mini *shell);

//initialize.c
void	init(t_mini *shell, char **env);
bool	create_cmd_list(char *input, t_mini *shell);
void	create_exec_cmd(t_exec_cmd *exec, t_token *token);

//mini.c
void	create_exec_cmds(t_mini *shell, t_cmd *cmd);
void	minishell(t_mini *shell);

//parse.c
bool	parse(char **input, t_mini *shell);
bool	is_open_quotes(char *line);
bool	add_string_middle(char **s, char *add, int pos);
void	add_space_after(char **s, char *delimiters);
void	add_space_before(char **str, char *delimiters);

//pipe.c
int		ft_pipe(t_mini *shell);
void	create_pipes(t_cmd *cmd);
bool	find_pipe_sequence(t_cmd *cmd);

//redirect
bool	redir(t_mini *shell, t_exec_cmd *cmd, t_token *token_redir);

//reset.c
void	ft_close(int fd);
void	reset_loop(t_mini *shell, char **input);
void	reset_cmd(t_mini *shell);

//signal.c
void	init_signal(void);
void	signal_int(int sig);
void	signal_quit(int sig);
void	signal_chld(void);

//token.c
void	create_tokens(t_cmd *cmd, t_mini *shell);
int		count_link_list(t_token *token);

// token_util.c
bool	is_blanked(char *str);
bool	is_redirect_type(t_type type);
char	*remove_quotes(char *str);
void	type_tokens(t_token **tokens);
void	get_next_type(t_token	**token, t_type type);

//util_free1.c
char	**free_array(char **array);
void	free_array_int(int **array, size_t n);
void	free_shell(t_mini *shell);
int		print_error(char *str, int num);
int		error_msg(char *str1, char *str2, char *str3, int ret);

//util_free2.c
void	free_exec_cmd(t_exec_cmd *exec);
void	free_node(t_env *env);
void	free_env(t_env *env);
void	free_cmds(t_cmd *cmds);

//util_join.c
void	ft_join_free(char **s1, char *s2);

//util_split.c
char	**ft_split_special(const char *s, char *c);
int	joint_into_array_arg(char ***array, t_token *token);
void	join_into_str(char **str, char **array, char *delimitador);

//util.c
int		is_redirect(const char *str);
bool	is_delimiter(char c, const char *delimiters);
bool	is_blanked(char *str);
void	cleanup_heredoc_files(void);

#endif