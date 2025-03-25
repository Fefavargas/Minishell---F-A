/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:48:31 by fefa              #+#    #+#             */
/*   Updated: 2025/03/24 22:15:25 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_token(t_mini *shell, t_token	*token)
{
	(void)shell;
	(void)token;
}

void	exec_start(t_mini *shell, t_token	*token)
{
	shell->pid = 0;
	redir(shell, token);
	if (token->next && shell->pid == P_CHILD)
		exec_start(shell, token->next);
	if ((!token->prev || token->prev->type == PIPE) && shell->pid == P_CHILD)
		exec_token(shell, token);
}

void	get_next_cmd(t_token	**token)
{
	t_token	*start;

	start = *token;
	while (start)
	{
		if (start->type == CMD)
			return ;
		start = start->next;
	}
}

void	exec_sort_token(t_mini *shell, t_cmd *cmd)
{
	t_token	*token;

	token = cmd->tokens;
	get_next_cmd(&token);
	while (!shell->exit && token)
	{
		exec_start(shell, token);
		token = token->next;
		get_next_cmd(&token);
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