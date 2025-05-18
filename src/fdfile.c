/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdfile.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 17:49:24 by albermud          #+#    #+#             */
/*   Updated: 2025/05/18 19:23:05 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	duplicate_fd(t_exec_cmd *exec)
{
	dup2(exec->fdout, STDOUT_FILENO);
	ft_close(exec->fdout);
	dup2(exec->fdin, STDIN_FILENO);
	ft_close(exec->fdin);
}

void	prepare_parent(int *pid, t_exec_cmd *exec)
{
	*pid = g_sig.sigchld;
	ft_close(exec->fdin);
	ft_close(exec->fdout);
}

void	close_pipes(t_cmd	*cmd)
{
	size_t	i;

	i = 0;
	while (i < cmd->n_pipes)
	{
		ft_close(cmd->fdpipe[i][0]);
		ft_close(cmd->fdpipe[i][1]);
		i++;
	}
}

void	update_fdin_fdout(t_exec_cmd **exec, t_cmd *cmd, int i, int n_pipes)
{
	if (i != 0)
		(*exec)->fdin = cmd->fdpipe[i - 1][0];
	if (i != n_pipes)
		(*exec)->fdout = cmd->fdpipe[i][1];
}

void	create_array_pids(t_cmd *cmd)
{
	size_t		i;
	t_exec_cmd	*exec;

	i = 0;
	exec = cmd->execcmd;
	while (exec)
	{
		if (!is_builtin(exec->cmd))
			cmd->n_binary++;
		exec = exec->next;
	}
	cmd->arr_pid = ft_calloc(cmd->n_binary, sizeof(int));
	while (i < cmd->n_binary)
		cmd->arr_pid[i++] = 0;
}
