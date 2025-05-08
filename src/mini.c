/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:48:31 by fefa              #+#    #+#             */
/*   Updated: 2025/05/08 09:28:04 by albermud         ###   ########.fr       */
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
	if (next && is_redirect_type(next->type) && shell->execution)
	{
		if (redir(shell, next))
		{
			shell->exit_code = 1;
			shell->execution = FALSE;
		}
		exec_start(shell, token, next->next);
	}
	else if (next && next->type != PIPE)
		exec_start(shell, token, next->next);
	else if (next && next->type == PIPE)
		pipe_flag = ft_pipe(shell);
	if ((!next || next->type == PIPE) && shell->execution)
	{
		get_next_cmd(&token);
		create_exec_cmd(&exec, token, shell);
		execute(shell, &exec);
		free_exec_cmd(&exec);
	}
	else if (!shell->execution && pipe_flag)
		shell->execution = TRUE;
	if (pipe_flag)
	{
		dup2(shell->pipin, STDIN_FILENO);
		close(shell->pipin);
		dup2(shell->stdout, STDOUT_FILENO);
		exec_start(shell, next->next, next->next);
	}
	else if (next && next->type == PIPE)
		exec_start(shell, next->next, next->next);
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


