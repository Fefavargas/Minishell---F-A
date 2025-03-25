/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 14:35:50 by fefa              #+#    #+#             */
/*   Updated: 2025/03/24 22:17:15 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redir_in(t_mini *shell, char *file)
{
	close(shell->fdin);
	shell->fdin = open(file, O_RDONLY);
	//if (shell->fdout < 0) //ERROR
	dup2(shell->fdin, STDIN_FILENO);
}

void	redir_out(t_mini *shell, t_type type_token, char *file)
{
	close(shell->fdout);
	if (type_token == TRUNC)
		shell->fdout = open(file, O_CREAT | O_WRONLY | O_TRUNC);
	else if (type_token == APPEND)
		shell->fdout = open(file, O_CREAT | O_WRONLY | O_APPEND);
	//if (shell->fdout < 0) //ERROR
	dup2(shell->fdout, STDOUT_FILENO);
}

void	get_prev_redir(t_token *prev, t_token *token_cmd)
{
	prev = token_cmd;
	while (prev && (prev->type != TRUNC && prev->type != APPEND && \
					prev->type != INPUT && prev->type != PIPE))
		prev = prev->prev;
}

void	redir(t_mini *shell, t_token *token_cmd)
{
	t_token	*prev;

	prev = NULL;
	get_prev_redir(prev, token_cmd);
	if (prev->next && prev->next->type == ARG)
	{
		if (prev->type == TRUNC)
			redir_in(shell, prev->next->str);
		else if ((prev->type == INPUT || prev->type == APPEND))
			redir_out(shell, prev->type, prev->next->str);
	}
	else if (prev->type == PIPE)
		pipex(shell);
}

