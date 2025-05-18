/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:12:51 by fefa              #+#    #+#             */
/*   Updated: 2025/05/18 16:36:36 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_binary(t_mini *shell, t_exec_cmd *exec)
{
	char	*path;

	path = get_path_bin(shell->env, exec->cmd);
	if (!path)
		path = exec->cmd;
	execve(path, exec->args, shell->arr_env);
	exit(error_message(path));
}

static void	handle_execution(t_mini *shell, t_cmd *cmd,
							t_exec_cmd *exec, int *i)
{
	if (exec->args && exec->args[0] && is_builtin(exec->args[0]))
	{
		prepare_fd(exec);
		shell->exit_code = exec_builtin(shell, exec);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
	}
	else
	{
		g_sig.sigchld = fork();
		if (g_sig.sigchld == -1)
			return ;
		if (g_sig.sigchld == 0)
		{
			prepare_fd(exec);
			signal_chld();
			close_cmd(cmd);
			exec_binary(shell, exec);
		}
		else
			prepare_parent(&(cmd->arr_pid[(*i)++]), exec);
	}
}

void	execute(t_mini *shell, t_cmd *cmd)
{
	t_exec_cmd	*exec;
	int			i;

	exec = cmd->execcmd;
	i = 0;
	while (exec)
	{
		if (exec->execution)
			handle_execution(shell, cmd, exec, &i);
		else if (shell->exit_code != 130)
			shell->exit_code = 1;
		exec = exec->next;
	}
	close_cmd(cmd);
	wait_fork(shell, cmd);
}
