/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvargas <fvargas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:12:51 by fefa              #+#    #+#             */
/*   Updated: 2025/05/14 23:43:48 by fvargas          ###   ########.fr       */
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

int	exec_binary(t_mini *shell, t_exec_cmd *exec, t_cmd *cmd, int i)
{
	char	*path;

	path = get_path_bin(shell->env, exec->cmd);
	if (!path)
		path = exec->cmd;
	g_sig.sigchld = fork();
	if (g_sig.sigchld == -1)
		return (1);
	if (g_sig.sigchld == 0)
	{
		execve(path, exec->args, shell->arr_env);
		exit(error_message(path));
	}
	else
		cmd->arr_pid[i] = g_sig.sigchld;
	return (0);
}

void	dup_fd(t_mini *shell, t_exec_cmd *exec)
{
	(void)shell;
	dup2(exec->fdout, STDOUT_FILENO);
	ft_close(exec->fdout);
	dup2(exec->fdin, STDIN_FILENO);
	ft_close(exec->fdin);
}

void	wait_fork(t_mini *shell, t_cmd *cmd)
{
	int		status;
	size_t	i;

	status = 0;
	i = 0;
	while (i <= cmd->n_pipes + 1 && g_sig.sigchld != 0)
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
			shell->exit_code = (128 + WTERMSIG(status));
		}
	}
}

void	create_array_pids(t_cmd *cmd)
{
	size_t	i;

	i = 0;
	cmd->arr_pid = ft_calloc(sizeof(int), cmd->n_pipes + 1);
	while (i < cmd->n_pipes + 1)
	{
		cmd->arr_pid[i] = 0;
		i++;
	}
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

void	close_all_fd(t_exec_cmd *exec)
{
	ft_close(exec->fdin);
	ft_close(exec->fdout);
}

void	execute(t_mini *shell, t_cmd *cmd)
{
	t_exec_cmd	*current;
	int			i;

	current = cmd->execcmd;
	i = 0;
	while (current)
	{
		if (current->execution)
		{
			dup_fd(shell, current);
			close_cmd(cmd);
			if (current->args && current->args[0] && is_builtin(current->args[0]))
				shell->exit_code = exec_builtin(shell, current);
			else
				shell->exit_code = exec_binary(shell, current, cmd, i);
		}
		else
			shell->exit_code = 1;
		current = current->next;
		i++;
	}
	free_exec_cmd(cmd->execcmd);
	wait_fork(shell, cmd);
}





// void	execute(t_mini *shell, t_cmd *cmd)
// {
// 	t_exec_cmd	*current;
// 	int			i;
// 	char		*path;
// 	bool		ret;

// 	current = cmd->execcmd;
// 	i = 0;
// 	while (current)
// 	{
// 		if (current->execution)
// 		{
// 			g_sig.sigchld = fork();
// 			if (g_sig.sigchld == -1)
// 				return ;
// 			if (g_sig.sigchld == 0)
// 			{
// 				dup_fd(shell, current);
// 				close_cmd(cmd);
// 				if (current->args && current->args[0] && is_builtin(current->args[0]))
// 				{
// 					ret = exec_builtin(shell, current);
// 					// free_shell(shell, current);
// 					// exit(ret);
// 					shell->exit_code = ret;
// 				}
// 				else
// 				{
// 					path = get_path_bin(shell->env, current->cmd);
// 					if (!path)
// 						path = current->cmd;
// 					execve(path, current->args, shell->arr_env);
// 					exit(error_message(path));
// 				}
// 			}
// 			else
// 			{
// 				cmd->arr_pid[i] = g_sig.sigchld;
// 				close_all_fd(current);
// 			}
// 		}
// 		else
// 			shell->exit_code = 1;
// 		current = current->next;
// 		i++;
// 	}
// 	free_exec_cmd(cmd->execcmd);
// 	wait_fork(shell, cmd);
// }

