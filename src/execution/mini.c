/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:48:31 by fefa              #+#    #+#             */
/*   Updated: 2025/05/18 21:33:23 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_exec_cmd_end(t_exec_cmd **first, t_exec_cmd *new)
{
	t_exec_cmd	*tmp;

	if (!new)
		return ;
	if (!new->cmd)
	{
		free_exec_cmd(new);
		return ;
	}
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
	exec->str = NULL;
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
	if (exec->args[1])
		join_into_str(&exec->str, &exec->args[1], " ");
}

static t_exec_cmd	*init_exec_cmd(t_token *token, t_cmd *cmd, size_t i)
{
	t_exec_cmd	*exec;

	exec = ft_calloc(sizeof(t_exec_cmd), 1);
	if (!exec)
		return (NULL);
	create_exec_cmd(exec, token);
	update_fdin_fdout(&exec, cmd, i, cmd->n_pipes);
	return (exec);
}

void	create_exec_cmds(t_mini *shell, t_cmd *cmd)
{
	t_token		*token;
	t_exec_cmd	*exec;
	size_t		i;

	i = 0;
	token = cmd->tokens;
	while (i < cmd->n_pipes + 1)
	{
		exec = init_exec_cmd(token, cmd, i++);
		if (!exec)
			return ;
		while (token && token->type != PIPE)
		{
			redir(shell, exec, token);
			token = token->next;
		}
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
	reset_cmd_list(shell);
}
