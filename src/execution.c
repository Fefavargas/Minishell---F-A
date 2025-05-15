/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albbermu <albbermu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:12:51 by fefa              #+#    #+#             */
/*   Updated: 2025/05/15 19:14:36 by albbermu         ###   ########.fr       */
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

	status = 0;
	i = 0;
	while (i <= cmd->n_pipes && g_sig.sigchld != 0)
	{
		if (cmd->arr_pid[i] != 0)
			waitpid(cmd->arr_pid[i], &status, 0);
		i++;
	}
	if (g_sig.sigchld != 0)
	{
		if (WIFEXITED(status))
			shell->exit_code = (WEXITSTATUS(status));
		if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGPIPE)
				ft_putstr_fd(" Broken pipe\n", STDERR_FILENO);
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
	t_exec_cmd	*current;
	int			i;
	bool		ret;

	current = cmd->execcmd;
	i = 0;
	while (current)
	{
		if (!ft_strcmp(current->cmd, "exit"))
			ft_exit(shell, current->args);
		if (current->execution)
		{
			g_sig.sigchld = fork();
			if (g_sig.sigchld == -1)
				return ;
			if (g_sig.sigchld == 0)
			{
				prepare_chld(shell, current, cmd);
				if (current->args && current->args[0] && is_builtin(current->args[0]))
				{
					ret = exec_builtin(shell, current);
					//free_shell(shell, current);
					exit(ret);
				}
				else
					exec_binary(shell, current);
			}
			else
				prepare_parent(&(cmd->arr_pid[i++]), current);
		}
		else if (shell->exit_code != 130)
			shell->exit_code = 1;
		current = current->next;
	}
	free_exec_cmd(cmd->execcmd);
	wait_fork(shell, cmd);
}
