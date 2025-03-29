/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:48:31 by fefa              #+#    #+#             */
/*   Updated: 2025/03/28 23:11:22 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_token(t_mini *shell, t_token	*token)
{
	t_exec_cmd exec;

	printf("Initializion exec_token: %s\n", token->str); //DELETE LATER
	create_exec_cmd(&exec, token);
	printf("exec_token cmd: %s \n", exec.cmd); //DELETE LATER
	printf("arg[0]: %s \n", exec.args[0]); //DELETE LATER
	printf("str: %s\n", exec.str); //DELETE LATER
	execute(shell, &exec);
}

void	exec_start(t_mini *shell, t_token	*token)
{
	printf("Initializion exec_start: %s\n", token->str); //DELETE LATER
	redir(shell, token);
	// if (token->next && shell->pid != P_PARENT)
	// 	exec_start(shell, token->next);
	// if ((!token->prev || token->prev->type == PIPE) && shell->pid != P_PARENT)
	exec_token(shell, token);
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

void	exec_sort_token(t_mini *shell, t_cmd *cmd)
{
	t_token	*token;

	token = cmd->tokens;
	printf("Initializion exec_sort_token\n"); //DELETE LATER
	get_next_cmd(&token);
	printf("start token cmd %s\n", token->str); //DELETE LATER
	while (!shell->exit && token)
	{
		exec_start(shell, token);
		token = token->next;
		get_next_cmd(&token);
	}
	shell->exit = TRUE; //DELETE LATER
}

void	minishell(t_mini *shell)
{
	t_cmd	*current;

	current = shell->cmd;
	while (!shell->exit && current)
	{
		exec_sort_token(shell, current);
		//reset_std(shell);
		current = current->next;
	}
}
