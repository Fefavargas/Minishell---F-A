/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 20:03:49 by fefa              #+#    #+#             */
/*   Updated: 2025/03/23 22:36:56 by fefa             ###   ########.fr       */
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
	shell->pid = -1;
	shell->pipin = -1;
	shell->pipout = -1;
}

void	reset_std(t_mini *shell)
{
	dup2(STDIN_FILENO, shell->fdin);
	dup2(STDOUT_FILENO, shell->fdout);
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
		free(cmd->cmd);
		while (cmd->tokens)
		{
			token = cmd->tokens;
			cmd->tokens = token->next;
			free(token->str);
			free(token);
		}
		free(cmd);
	}
	shell->cmd = NULL;
}
