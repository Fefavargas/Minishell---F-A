/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvargas <fvargas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:48:31 by fefa              #+#    #+#             */
/*   Updated: 2025/04/23 16:20:34 by fvargas          ###   ########.fr       */
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
	pipe_flag = -1;
	if (next && is_redirect(next->type))
	{
		if (!redir(shell, next))
			return ;
		exec_start(shell, token, next->next);
	}
	else if (next && next->type != PIPE)
		exec_start(shell, token, next->next);
	else if (!next || next->type == PIPE)
	{
		if (next && next->type == PIPE)
			pipe_flag = ft_pipe(shell);
		if (!next || pipe_flag == P_CHILD)
		{
			get_next_cmd(&token);
			create_exec_cmd(&exec, token, shell);
			execute(shell, &exec);
			free_exec_cmd(&exec);
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
		current = current->next;
	}
}
