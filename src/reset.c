/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvargas <fvargas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 20:03:49 by fefa              #+#    #+#             */
/*   Updated: 2025/05/13 18:51:57 by fvargas          ###   ########.fr       */
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

void	reset_loop(t_mini *shell, char *input)
{
	reset_std(shell);
	free(input);
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
		while (cmd->tokens)
		{
			token = cmd->tokens;
			cmd->tokens = token->next;
			if (token->str)
				free(token->str);
			free(token);
		}
		free_array_int(cmd->fdpipe);
		free(cmd);
	}
	shell->cmd = NULL;
}
