/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 16:36:42 by albermud          #+#    #+#             */
/*   Updated: 2025/05/18 17:03:34 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper function to report syntax errors for redirection
static bool	report_redirection_syntax_error(t_token *token)
{
	if (!token->next || (token->next->type != FILENAME
			&& token->next->type != DELIMITER))
		return (error_msg("", "", \
			": syntax error near unexpected token `newline'\n", 1));
	return (false);
}

// Helper function to perform the actual redirection
static bool	perform_redir(t_mini *shell, t_exec_cmd *exec, t_token *token)
{
	int		fd;

	if (token->type == INPUT)
		return (redir_in(&exec->fdin, token->next->str));
	else if (token->type == TRUNC || token->type == APPEND)
		return (redir_out(&exec->fdout, shell->env,
				token->type, token->next->str));
	else if (token->type == HEREDOC)
	{
		fd = heredoc(shell, token);
		if (fd >= 0)
		{
			if (exec->fdin > 0 && exec->fdin != STDIN_FILENO)
				ft_close(exec->fdin);
			exec->fdin = fd;
			return (0);
		}
		return (1);
	}
	return (1);
}

bool	redir(t_mini *shell, t_exec_cmd *exec, t_token *token)
{
	bool	ret;

	if (!token || !is_redirect_type(token->type))
		return (true);
	if (!exec->execution && token->type != HEREDOC)
		return (true);
	if (report_redirection_syntax_error(token))
		ret = true;
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
