/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 20:03:49 by fefa              #+#    #+#             */
/*   Updated: 2025/05/16 19:09:43 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_close(int fd)
{
	if (fd > 0)
		close(fd);
}

void	reset_std(t_mini *shell)
{
	dup2(shell->stdin, STDIN_FILENO);
	dup2(shell->stdout, STDOUT_FILENO);
}

void	reset_loop(t_mini *shell, char **input)
{
	cleanup_heredoc_files();
	reset_cmd(shell);
	reset_std(shell);
	if (*input)
		free(*input);
	*input = NULL;
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
			free(cmd->cmd);
		if (cmd->words)
			cmd->words = free_array(cmd->words);
		if (!cmd->n_pipes)
			free_array_int(cmd->fdpipe, cmd->n_pipes);
		while (cmd->tokens)
		{
			token = cmd->tokens;
			cmd->tokens = token->next;
			if (token->str)
				free(token->str);
			free(token);
		}
		if (cmd->arr_pid)
			free(cmd->arr_pid);
		free(cmd);
	}
	shell->cmd = NULL;
}
