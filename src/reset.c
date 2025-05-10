/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 20:03:49 by fefa              #+#    #+#             */
/*   Updated: 2025/05/10 09:31:19 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_close(int fd)
{
	if (fd > 0)
		close(fd);
}

void	reset_fds(t_mini *shell, bool close)
{
	if (close)
	{
		ft_close(shell->fdin);
		ft_close(shell->fdout);
		ft_close(shell->pipin);
		ft_close(shell->pipout);
	}
	shell->fdin = -1;
	shell->fdout = -1;
	shell->pipin = -1;
	shell->pipout = -1;
}

void	reset_std(t_mini *shell)
{
	dup2(shell->stdin, STDIN_FILENO);
	dup2(shell->stdout, STDOUT_FILENO);
}

void	reset_loop(t_mini *shell)
{
	reset_fds(shell, TRUE);
	reset_std(shell);
}

void	reset_cmd(t_mini *shell)
{
	t_cmd	*cmd;
	t_token	*token;

	while (shell->cmd)
	{
		cmd = shell->cmd;
		shell->cmd = cmd->next;
		if (cmd->cmd)
		{
			free(cmd->cmd);
			cmd->cmd = NULL;
		}
		if (cmd->words)
		{
			free_array(cmd->words);
			cmd->words = NULL;
		}
		while (cmd->tokens)
		{
			token = cmd->tokens;
			cmd->tokens = token->next;
			if (token->str)
				free(token->str);
			free(token);
		}
		free(cmd);
	}
	shell->cmd = NULL;
	shell->execution = TRUE;
}
