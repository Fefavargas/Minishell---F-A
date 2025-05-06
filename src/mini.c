/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:48:31 by fefa              #+#    #+#             */
/*   Updated: 2025/05/06 07:34:00 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_token(t_mini *shell, t_token *token)
{
	t_exec_cmd	exec;

	exec = (t_exec_cmd){0};
	create_exec_cmd(&exec, token, shell);
	execute(shell, &exec);
	free_exec_cmd(&exec);
}

void	get_next_cmd(t_token	**token)
{
	while (*token)
	{
		if ((*token)->type == CMD)
			return ;
		*token = (*token)->next;
	}
}

void	exec_start(t_mini *shell, t_token *token, t_token	*next)
{
	t_exec_cmd	exec;
	bool		pipe_flag;

	exec = (t_exec_cmd){0};
	pipe_flag = 0;
	if (next && is_redirect_type(next->type))
	{
		if (redir(shell, next))
		{
			shell->exit_code = 1;
			return ;
		}
		exec_start(shell, token, next->next);
		return ;
	}
	else if (next && next->type != PIPE)
		exec_start(shell, token, next->next);
	else if (next && next->type == PIPE)
		pipe_flag = ft_pipe(shell);
	if (!next || pipe_flag == 1)
	{
		get_next_cmd(&token);
		create_exec_cmd(&exec, token, shell);
		execute(shell, &exec);
		free_exec_cmd(&exec);
		if (pipe_flag)
		{
			dup2(shell->pipin, STDIN_FILENO);
			close(shell->pipin);
			dup2(shell->stdout, STDOUT_FILENO);
			exec_start(shell, next->next, next->next);
		}
	}
}

void	minishell(t_mini *shell)
{
	t_cmd	*current;

	current = shell->cmd;
	while (!shell->exit && current)
	{
		exec_start(shell, current->tokens, current->tokens);
		if (g_sig.sigchld == 0)
		{
			free_shell(shell);
			exit(g_sig.sigexit);
		}
		current = current->next;
	}
}
