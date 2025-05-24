/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 16:36:42 by albermud          #+#    #+#             */
/*   Updated: 2025/05/24 14:54:12 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	redir_out(int *fdout, t_env *env, t_type type_token, char *file)
{
	int	fd;

	if (access(file, F_OK) == 0 && access(file, W_OK) < 0)
		return (error_msg("", file, ": Permission denied\n", 1));
	if (ensure_directory_exists(env, file))
		return (error_msg("", file, ": Cannot create directory\n", 1));
	if (type_token == TRUNC)
		fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (type_token == APPEND)
		fd = open(file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd < 0)
		return (print_error("Error opening outfile", 1));
	*fdout = fd;
	return (0);
}

bool	redir_in(int *fdin, char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (error_msg("", file, ": No such file or directory\n", 1));
	*fdin = fd;
	return (0);
}

// Helper function to perform the actual redirection
static bool	perform_redir(t_mini *shell, t_exec_cmd *exec, t_token *token)
{
	if (token->type == INPUT)
		return (redir_in(&exec->fdin, token->next->str));
	else if (token->type == TRUNC || token->type == APPEND)
		return (redir_out(&exec->fdout, shell->env,
				token->type, token->next->str));
	else if (token->type == HEREDOC)
		return (heredoc(shell, token, exec));
	return (1);
}

// Helper function to report syntax errors for redirection
static bool	report_redirection_syntax_error(t_token *token)
{
	if (!token->next || (token->next->type != FILENAME
			&& token->next->type != DELIMITER))
		return (error_msg("", "", \
			": syntax error near unexpected token `newline'\n", 1));
	return (0);
}

bool	redir(t_mini *shell, t_exec_cmd *exec, t_token *token)
{
	bool	ret;

	if (!token || !is_redirect_type(token->type))
		return (1);
	if (!exec->execution && token->type != HEREDOC)
		return (1);
	if (report_redirection_syntax_error(token))
		ret = 1;
	else
		ret = perform_redir(shell, exec, token);
	if (ret)
	{
		if (shell->exit_code != 130)
			shell->exit_code = 1;
		exec->execution = false;
	}
	return (ret);
}
