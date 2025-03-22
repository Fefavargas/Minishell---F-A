/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:48:31 by fefa              #+#    #+#             */
/*   Updated: 2025/03/21 17:22:52 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_start(t_mini *shell, t_token	*token)
{
	int	pipe;

	redir(shell, token);
	pipe = P_CHILD;
	if (token->type == PIPE)
		pipe = pipex(shell);
	if (token->next && pipe == P_CHILD)
		exec_start(shell, token->next);
	//if ((!token->prev || token->prev->type == PIPE) && pipe == P_CHILD)
	//	exec_token(shell, token);
}

void	get_next_cmd(t_token	**token)
{
	t_token	*start;
	
	start = *token;
	while (start)
	{
		if (start->type == CMD)
			return;
		start = start->next;
	}
}

void	exec_sort_token(t_mini *shell, t_cmd *cmd)
{
	t_token	*token;

	token = cmd->tokens;
	while(!shell->exit)
	{
		get_next_cmd(&token);
		exec_start(shell, token);
	}
}

void	minishell(t_mini *shell)
{
	t_cmd	*current;

	current = shell->cmd;
	while (!shell->exit && current)
	{
		exec_sort_token(shell, current);
		// reset_std(shell);
		// current = current->next;
	}
}