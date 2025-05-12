/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 14:35:50 by fefa              #+#    #+#             */
/*   Updated: 2025/05/12 16:20:30 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	create_directory(t_env *env, char *path_copy)
{
	int		status;
	pid_t	pid;
	char	*path_bin;
	char	*arr[3];

	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
	{
		arr[0] = "mkdir";
		arr[1] = path_copy;
		arr[2] = NULL;
		path_bin = get_path_bin(env, "mkdir");
		execve(path_bin, arr, NULL);
		exit(error_message(path_bin));
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
		return (0);
	return (1);
}

bool	ensure_directory_exists(t_env *env, const char *path)
{
	char		*path_copy;
	char		*last_slash;
	struct stat	st;

	path_copy = ft_strdup(path);
	if (!path_copy)
		return (1);
	last_slash = ft_strrchr(path_copy, '/');
	if (last_slash)
	{
		*last_slash = '\0';
		if (stat(path_copy, &st) != 0 || !S_ISDIR(st.st_mode))
		{
			if (create_directory(env, path_copy))
			{
				free(path_copy);
				return (1);
			}
		}
	}
	free(path_copy);
	return (0);
}

bool	redir_out(t_mini *shell, t_type type_token, char *file)
{
	if (access(file, F_OK) == 0 && access(file, W_OK) < 0)
		return (error_msg("", file, ": Permission denied\n", 1));
	if (ensure_directory_exists(shell->env, file))
		return (error_msg("", file, ": Cannot create directory\n", 1));
	shell->fdout = -1;
	if (type_token == TRUNC)
		shell->fdout = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (type_token == APPEND)
		shell->fdout = open(file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (shell->fdout < 0)
		return (print_error("Error opening outfile", 1));
	if (dup2(shell->fdout, STDOUT_FILENO) < 0)
	{
		ft_close(shell->fdout);
		return (print_error("Error duplicating file descriptor for output", 1));
	}
	ft_close(shell->fdout);
	return (0);
}

bool	redir_in(t_mini *shell, char *file)
{
	shell->fdin = open(file, O_RDONLY);
	if (shell->fdin < 0)
		return (error_msg("", file, ": No such file or directory\n", 1));
	if (dup2(shell->fdin, STDIN_FILENO) < 0)
	{
		ft_close(shell->fdin);
		return (print_error("Error duplicating file descriptor to input\n", 1));
	}
	ft_close(shell->fdin);
	return (0);
}

bool	redir(t_mini *shell, t_token *token_redir)
{
	bool	ret;

	ret = 1;
	if (!token_redir || !is_redirect_type(token_redir->type))
		error_msg("", "", ": syntax error near unexpected token\n", 0);
	else if (!token_redir->next || (token_redir->next->type != FILENAME && \
									token_redir->next->type != DELIMITER))
		error_msg("", "", ": syntax error near unexpected token `newline'", 0);
	else if (token_redir->type == INPUT)
		ret = redir_in(shell, token_redir->next->str);
	else if (token_redir->type == HEREDOC)
		ret = heredoc(shell, token_redir);
	else if (token_redir->type == TRUNC || token_redir->type == APPEND)
		ret = redir_out(shell, token_redir->type, token_redir->next->str);
	if (ret)
	{
		shell->exit_code = 1;
		shell->execution = FALSE;
	}
	return (1);
}
