/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvargas <fvargas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:12:51 by fefa              #+#    #+#             */
/*   Updated: 2025/05/13 19:54:12 by fvargas          ###   ########.fr       */
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
				return (error_msg("", path, ": Is a directory", 126));
			else if (access(path, X_OK) == -1)
				return (error_msg("", path, ": Permission denied", 126));
		}
		return (error_msg("", path, ": No such file or directory", 127));
	}
	return (error_msg("", path, ": command not found\n", 127));
}

void	exec_binary(t_mini *shell, t_exec_cmd *exec)
{
	char	*path;

	path = get_path_bin(shell->env, exec->cmd);
	if (!path)
		path = exec->cmd;
	g_sig.sigchld = fork();
	if (g_sig.sigchld == -1)
		return ;
	if (g_sig.sigchld == 0)
	{
		if (execve(path, exec->args, shell->arr_env) == -1)
			exit(error_message(path));
	}
}

void	dup_fd(t_mini *shell, t_exec_cmd *current)
{
	if (dup2(current->fdout, STDOUT_FILENO) < 0)
	{
		ft_close(current->fdout);
		print_error("Error duplicating file descriptor for output", 1);
		shell->exit_code = 1;
		return ;
	}
	ft_close(current->fdout);
	if (dup2(current->fdin, STDIN_FILENO) < 0)
	{
		ft_close(current->fdin);
		shell->exit_code = 1;
		print_error("Error duplicating file descriptor to input\n", 1);
		return ;
	}
	ft_close(current->fdin);
}

void	wait_fork(t_mini *shell)
{
	int			status;

	status = 0;
	if (g_sig.sigchld != 0)
	{
		waitpid(g_sig.sigchld, &status, 0);
		if (WIFEXITED(status))
			shell->exit_code = (WEXITSTATUS(status));
		if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGPIPE)
				ft_putstr_fd(" Broken pipe\n", STDERR_FILENO);
			shell->exit_code = (128 + WTERMSIG(status));
		}
	}
}

void	execute(t_mini *shell, t_exec_cmd *exec)
{
	t_exec_cmd	*current;

	current = exec;
	while (current)
	{
		if (current->execution)
		{
			dup_fd(shell, current);
			if (!current->cmd)
				shell->exit_code = 0;
			else if (is_builtin(current->args[0]))
				shell->exit_code = exec_builtin(shell, current);
			else if (current->args[0] && current->args[0][0])
				exec_binary(shell, current);
			else
				shell->exit_code = 0;
		}
		current = current->next;
	}
	free_exec_cmd(exec);
	wait_fork(shell);
}
