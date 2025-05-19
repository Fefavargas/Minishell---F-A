/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_wait_fork.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 18:41:21 by albermud          #+#    #+#             */
/*   Updated: 2025/05/18 19:40:50 by fefa             ###   ########.fr       */
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

static void	handle_exit_status(t_mini *shell, int status)
{
	if (WIFEXITED(status))
		shell->exit_code = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGPIPE)
			ft_putstr_fd(" Broken pipe\n", STDERR_FILENO);
		else if (WTERMSIG(status) == SIGQUIT)
			ft_putstr_fd("Quit: (core dumped)\n", STDERR_FILENO);
		shell->exit_code = 128 + WTERMSIG(status);
	}
}

static void	wait_for_children(t_cmd *cmd, int *status)
{
	size_t	i;

	i = 0;
	while (i < cmd->n_binary && g_sig.sigchld != 0)
	{
		if (cmd->arr_pid[i] != 0)
			waitpid(cmd->arr_pid[i], status, 0);
		i++;
	}
}

void	wait_fork(t_mini *shell, t_cmd *cmd)
{
	int	status;

	status = 0;
	wait_for_children(cmd, &status);
	if (g_sig.sigchld != 0)
		handle_exit_status(shell, status);
	g_sig.sigchld = 0;
	signal(SIGINT, signal_int);
	signal(SIGQUIT, SIG_IGN);
}
