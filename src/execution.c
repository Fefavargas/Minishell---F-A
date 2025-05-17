/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:12:51 by fefa              #+#    #+#             */
/*   Updated: 2025/05/17 11:16:44 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	error_message(char *path)
{
	struct stat	path_stat;

	if (!path || ft_strlen(path) == 0)
		return (error_msg("", "", ": command not found\n", 127));
	if ((path[0] == '.' && path[1] == '/') || path[0] == '/')
	{
		if (stat(path, &path_stat) == 0)
		{
			if (S_ISDIR(path_stat.st_mode))
				return (error_msg("", path, ": Is a directory\n", 126));
			else if (access(path, X_OK) == -1)
				return (error_msg("", path, ": Permission denied\n", 126));
		}
		return (error_msg("", path, ": No such file or directory\n", 127));
	}
	return (error_msg("", path, ": command not found\n", 127));
}

int	exec_binary(t_mini *shell, t_exec_cmd *exec)
{
	char	*path;

	path = get_path_bin(shell->env, exec->cmd);
	if (!path)
		path = exec->cmd;
	execve(path, exec->args, shell->arr_env);
	exit(error_message(path));
}

void	wait_fork(t_mini *shell, t_cmd *cmd)
{
	int		status;
	size_t	i;
	int		last_cmd_idx;

	status = 0;
	i = 0;
	last_cmd_idx = -1;
	while (i <= cmd->n_pipes)
	{
		if (cmd->arr_pid[i] != 0)
		{
			last_cmd_idx = i;
			waitpid(cmd->arr_pid[i], &status, 0);
		}
		i++;
	}
	if (last_cmd_idx != -1)
	{
		if (WIFEXITED(status))
			shell->exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGPIPE)
				ft_putstr_fd("", STDERR_FILENO);
			else if (WTERMSIG(status) == SIGQUIT)
				ft_putstr_fd("Quit: (core dumped)\n", STDERR_FILENO);
			shell->exit_code = (128 + WTERMSIG(status));
		}
	}
	g_sig.sigchld = 0;
	signal(SIGINT, signal_int);
	signal(SIGQUIT, SIG_IGN);
}

void	execute(t_mini *shell, t_cmd *cmd)
{
	t_exec_cmd	*current_exec;
	t_exec_cmd	*last_pipeline_cmd = NULL;
	int			pid_arr_idx;
	bool		builtin_ret_val;

	if (cmd->execcmd)
	{
		last_pipeline_cmd = cmd->execcmd;
		while (last_pipeline_cmd->next)
			last_pipeline_cmd = last_pipeline_cmd->next;
	}
	current_exec = cmd->execcmd;
	pid_arr_idx = 0;
	while (current_exec)
	{
		if (!ft_strcmp(current_exec->cmd, "exit") && cmd->n_pipes == 0 && is_builtin("exit"))
		{
			if (current_exec->execution)
				exec_builtin(shell, current_exec);
		}
		else if (current_exec->execution)
		{
			g_sig.sigchld = fork();
			if (g_sig.sigchld == -1)
				error_msg("minishell: fork: ", strerror(errno), "", 1);
			else if (g_sig.sigchld == 0)
			{
				prepare_chld(shell, current_exec, cmd);
				if (current_exec->args && current_exec->args[0] && is_builtin(current_exec->args[0]))
				{
					builtin_ret_val = exec_builtin(shell, current_exec);
					exit(builtin_ret_val);
				}
				else
					exec_binary(shell, current_exec);
			}
			else
				prepare_parent(&(cmd->arr_pid[pid_arr_idx]), current_exec);
		}
		pid_arr_idx++;
		current_exec = current_exec->next;
	}
	wait_fork(shell, cmd);
	if (last_pipeline_cmd && !last_pipeline_cmd->execution)
		shell->exit_code = 1;
	free_exec_cmd(cmd->execcmd);
	cmd->execcmd = NULL;
}
