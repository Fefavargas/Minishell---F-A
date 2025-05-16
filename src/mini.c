/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:48:31 by fefa              #+#    #+#             */
/*   Updated: 2025/05/16 19:40:13 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_exec_cmd_end(t_exec_cmd **first, t_exec_cmd *new)
{
	t_exec_cmd	*tmp;

	if (!new)
		return ;
	if (!(*first))
	{
		*first = new;
		return ;
	}
	tmp = *first;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

void	create_exec_cmd(t_exec_cmd *exec, t_token *token)
{
	get_next_type(&token, CMD);
	exec->args = NULL;
	exec->str = NULL; //DELETE LATER
	exec->cmd = NULL;
	exec->execution = 1;
	exec->fdin = dup(STDIN_FILENO);
	exec->fdout = dup(STDOUT_FILENO);
	if (!token)
		return ;
	joint_into_array_arg(&exec->args, token);
	if (!exec->args)
		return ;
	if (exec->args[0] && exec->args[0][0])
	{
		exec->cmd = ft_strdup(exec->args[0]);
		if (!exec->cmd)
			return ;
	}
	if (exec->args[1]) //DELETE LATER
		join_into_str(&exec->str, &exec->args[1], " "); //DELETE LATER
}

void	update_fdin_fdout(t_exec_cmd **exec, t_cmd *cmd, int i, int n_pipes)
{
	if (i != 0)
	{
		// ft_close((*exec)->fdin);
		(*exec)->fdin = cmd->fdpipe[i - 1][0];
	}
	if (i != n_pipes)
	{
		// ft_close((*exec)->fdout);
		(*exec)->fdout = cmd->fdpipe[i][1];
	}
}

void	create_exec_cmds(t_mini *shell, t_cmd *cmd)
{
	t_token		*token;
	t_exec_cmd	*exec;
	size_t		i;

	i = 0;
	token = cmd->tokens;
	// here the while is to create exec_cmd, so n_pipe + 1
	while (i < cmd->n_pipes + 1)
	{
		exec = ft_calloc(sizeof(t_exec_cmd), 1);
		create_exec_cmd(exec, token);
		update_fdin_fdout(&exec, cmd, i++, cmd->n_pipes);
		while (token && token->type != PIPE)
		{
			redir(shell, exec, token);
			token = token->next;
		}
		if (exec->cmd)
			add_exec_cmd_end(&cmd->execcmd, exec);
		if (token)
			token = token->next;
	}
}

void	minishell(t_mini *shell)
{
	t_cmd	*current;

	current = shell->cmd;
	while (current)
	{
		if (!current->tokens)
			shell->exit_code = 0;
		else
		{
			create_pipes(current);
			create_exec_cmds(shell, current);
			create_array_pids(current);
			execute(shell, current);
		}
		current = current->next;
	}
	reset_cmd(shell);
}
