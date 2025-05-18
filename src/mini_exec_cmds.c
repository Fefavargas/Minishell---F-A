/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_exec_cmds.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 18:18:31 by albermud          #+#    #+#             */
/*   Updated: 2025/05/18 18:28:41 by albermud         ###   ########.fr       */
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

static void	handle_exec_or_cleanup(t_cmd *cmd, t_exec_cmd *exec)
{
	if (exec->cmd)
		add_exec_cmd_end(&cmd->execcmd, exec);
	else
	{
		ft_close(exec->fdin);
		ft_close(exec->fdout);
		free(exec->cmd);
		free_array(exec->args);
		free(exec->str);
		free(exec);
	}
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
		handle_exec_or_cleanup(cmd, exec);
		if (token)
			token = token->next;
	}
}
