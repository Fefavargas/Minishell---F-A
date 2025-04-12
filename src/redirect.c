/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 14:35:50 by fefa              #+#    #+#             */
/*   Updated: 2025/04/09 11:45:50 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redir_in(t_mini *shell, char *file)
{
	shell->fdin = open(file, O_RDONLY);
	if (shell->fdin < 0)
	{
		perror("Error opening infile");
		exit(1);
	}
	if (dup2(shell->fdin, STDIN_FILENO) < 0)
	{
		perror("Error duplicating file descriptor for input");
		ft_close(shell->fdin);
		exit(1);
	}
	ft_close(shell->fdin);
}

void	redir_out(t_mini *shell, t_type type_token, char *file)
{
	if (type_token == TRUNC)
		shell->fdout = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (type_token == APPEND)
		shell->fdout = open(file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (shell->fdout < 0)
	{
		perror("Error opening outfile");
		exit(1);
	}
	if (dup2(shell->fdout, STDOUT_FILENO) < 0)
	{
		perror("Error duplicating file descriptor for output");
		ft_close(shell->fdout);
		exit(1);
	}
	ft_close(shell->fdout);
}

/**
 * Function gets the previous tokes of this types  (TRUNC, APPEND, INPUT, PIPE)
 */
void	get_prev_redir(t_token **prev, t_token *token_cmd)
{
	t_token	*tmp;

	tmp = token_cmd;
	while (tmp && (tmp->type != TRUNC && tmp->type != APPEND && \
					tmp->type != INPUT && tmp->type != PIPE))
		tmp = tmp->prev;
	*prev = tmp;
}

void	redir(t_mini *shell, t_token *token_cmd)
{
	t_token	*prev;

	get_prev_redir(&prev, token_cmd);
	if (!prev || (prev->type != INPUT && prev->type != TRUNC && prev->type
			!= APPEND))
	{
		fprintf(stderr, "minishell: syntax error near unexpected token\n");
		return ;
	}
	if (!prev->next || prev->next->type != ARG)
	{
		fprintf(stderr, "minishell: syntax error near unexpected token "
			" `newline'\n");
		return ;
	}
	if (prev->type == INPUT)
		redir_in(shell, prev->next->str);
	else if (prev->type == TRUNC || prev->type == APPEND)
		redir_out(shell, prev->type, prev->next->str);
}
