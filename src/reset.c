/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 20:03:49 by fefa              #+#    #+#             */
/*   Updated: 2025/05/18 17:54:50 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reset_std(t_mini *shell)
{
	ft_close(STDIN_FILENO);
	ft_close(STDOUT_FILENO);
	dup2(shell->stdin, STDIN_FILENO);
	dup2(shell->stdout, STDOUT_FILENO);
}

void	reset_loop(t_mini *shell, char **input)
{
	unlink("tmp_file");
	reset_std(shell);
	if (*input)
		free(*input);
	*input = NULL;
	reset_cmd_list(shell);
}

static void	reset_cmd_tokens(t_cmd *cmd)
{
	t_token	*token;

	while (cmd->tokens)
	{
		token = cmd->tokens;
		cmd->tokens = token->next;
		if (token->str)
			free(token->str);
		free(token);
	}
}

static void	reset_cmd(t_cmd *cmd)
{
	if (cmd->execcmd)
	{
		free_exec_cmd(cmd->execcmd);
		cmd->execcmd = NULL;
	}
	if (cmd->cmd)
		free(cmd->cmd);
	if (cmd->words)
		cmd->words = free_array(cmd->words);
	if (cmd->n_pipes > 0 && cmd->fdpipe)
		free_array_int(cmd->fdpipe, cmd->n_pipes);
	reset_cmd_tokens(cmd);
	if (cmd->arr_pid)
	{
		free(cmd->arr_pid);
		cmd->arr_pid = NULL;
	}
}

void	reset_cmd_list(t_mini *shell)
{
	t_cmd	*cmd;

	while (shell->cmd)
	{
		cmd = shell->cmd;
		shell->cmd = cmd->next;
		reset_cmd(cmd);
		free(cmd);
	}
	shell->cmd = NULL;
}
