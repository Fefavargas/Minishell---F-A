/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdfile.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 17:49:24 by albermud          #+#    #+#             */
/*   Updated: 2025/05/18 18:14:18 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	prepare_fd(t_exec_cmd *exec)
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

void	close_cmd(t_cmd	*cmd)
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

void	ft_close(int fd)
{
	if (fd > 0)
		close(fd);
}
