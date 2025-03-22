/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 14:35:50 by fefa              #+#    #+#             */
/*   Updated: 2025/03/21 14:37:15 by fefa             ###   ########.fr       */
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

void	redir(t_mini *shell, t_token *token)
{
	if (token->next &&  token->next->type == ARG)
	{
		if (token->type == TRUNC  && token->next->type == ARG)
			redir_in(shell, token->next->str);
		else if ((token->type == INPUT || token->type == APPEND))
			redir_out(shell, token->type, token->next->str);
	}
}