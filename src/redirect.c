/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 14:35:50 by fefa              #+#    #+#             */
/*   Updated: 2025/05/05 18:22:06 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_redirect_type(t_type type)
{
	if (type == TRUNC || type == APPEND || type == INPUT || type == HEREDOC)
		return (true);
	return (false);
}

bool	redir_in(t_mini *shell, char *file)
{
	shell->fdin = open(file, O_RDONLY);
	if (shell->fdin < 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(file, STDERR_FILENO);
        ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		shell->exit_code = 1;
		return (ERROR);
	}
	if (dup2(shell->fdin, STDIN_FILENO) < 0)
	{
		perror("Error duplicating file descriptor for input");
		ft_close(shell->fdin);
		shell->exit_code = 1;
		//exit(1);
	}
	ft_close(shell->fdin);
	return (SUCCESS);
}

bool    redir_out(t_mini *shell, t_type type_token, char *file)
{
    if (access(file, F_OK) == 0 && access(file, W_OK) < 0)
    {
        fprintf(stderr, "minishell: %s: Permission denied\n", file);
        shell->exit_code = 1;
        return (ERROR);
    }
    shell->fdout = -1;
    if (type_token == TRUNC)
        shell->fdout = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    else if (type_token == APPEND)
        shell->fdout = open(file, O_CREAT | O_WRONLY | O_APPEND, 0644);
    if (shell->fdout < 0)
    {
        perror("Error opening outfile");
        shell->exit_code = 1;
        return (ERROR);
    }
    if (dup2(shell->fdout, STDOUT_FILENO) < 0)
    {
        perror("Error duplicating file descriptor for output");
        shell->exit_code = 1;
        ft_close(shell->fdout);
        return (ERROR);
    }
    ft_close(shell->fdout);
    return (SUCCESS);
}
/**
 * Function gets the next token of this types  (TRUNC, APPEND, INPUT, PIPE)
 */
void	get_next_redir(t_token **next, t_token *token_cmd)
{
	t_token	*tmp;

	tmp = token_cmd;
	while (tmp && !is_redirect_type(tmp->type) && tmp->type != PIPE)
		tmp = tmp->next;
	*next = tmp;
}

bool redir(t_mini *shell, t_token *token_redir)
{
	if (!token_redir || !is_redirect_type(token_redir->type))
	{
		fprintf(stderr, "minishell: syntax error near unexpected token\n");
		return (ERROR);
	}
	else if (!token_redir->next || (token_redir->next->type != FILENAME && token_redir->next->type != DELIMITER))
	{
		fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n");
		return (ERROR);
	}
	else if (token_redir->type == INPUT)
		return (redir_in(shell, token_redir->next->str));
	else if (token_redir->type == HEREDOC)
		return (heredoc(shell, token_redir));
	else if (token_redir->type == TRUNC || token_redir->type == APPEND)
		return (redir_out(shell, token_redir->type, token_redir->next->str));
	return (ERROR);
}

