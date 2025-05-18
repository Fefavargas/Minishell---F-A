/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 16:36:42 by albermud          #+#    #+#             */
/*   Updated: 2025/05/18 16:43:44 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper function to report syntax errors for redirection
static bool	report_redirection_syntax_error(t_token *token)
{
	if (!token || !is_redirect_type(token->type))
	{
		error_msg("", "", ": syntax error near unexpected token\n", 0);
		return (true);
	}
	else if (!token->next || (token->next->type != FILENAME
			&& token->next->type != DELIMITER))
	{
		error_msg("", "",
			": syntax error near unexpected token `newline'\n", 0);
		return (true);
	}
	return (false);
}

// Helper function to perform the actual redirection
static bool	perform_redirection(t_mini *shell, t_exec_cmd *exec,
									t_token *token)
{
	bool	specific_ret;
	int		fd;

	specific_ret = true;
	if (token->type == INPUT)
		specific_ret = redir_in(&exec->fdin, token->next->str);
	else if (token->type == HEREDOC)
	{
		fd = heredoc(shell, token);
		if (fd >= 0)
		{
			if (exec->fdin > 0 && exec->fdin != STDIN_FILENO)
				ft_close(exec->fdin);
			exec->fdin = fd;
			specific_ret = 0;
		}
		else
			specific_ret = 1;
	}
	else if (token->type == TRUNC || token->type == APPEND)
		specific_ret = redir_out(&exec->fdout, shell->env,
				token->type, token->next->str);
	return (specific_ret);
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
		ret = perform_redirection(shell, exec, token);
	if (ret)
	{
		if (shell->exit_code != 130)
			shell->exit_code = 1;
		exec->execution = FALSE;
	}
	return (ret);
}
