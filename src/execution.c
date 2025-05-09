/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvargas <fvargas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:12:51 by fefa              #+#    #+#             */
/*   Updated: 2025/05/09 18:11:55 by fvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	error_message(char *path)
{
	struct stat	path_stat;

	if (!path || ft_strlen(path) == 0)
	{
		ft_putstr_fd("minishell: : command not found\n", STDERR_FILENO);
		return (127);
	}
	if ((path[0] == '.' && path[1] == '/') || path[0] == '/')
	{
		if (stat(path, &path_stat) == 0)
		{
			if (S_ISDIR(path_stat.st_mode))
			{
				// Remove the special case for PWD - treat all directories the same
				ft_putstr_fd("minishell: ", STDERR_FILENO);
				ft_putstr_fd(path, STDERR_FILENO);
				ft_putendl_fd(": Is a directory", STDERR_FILENO);
			}
			else if (access(path, X_OK) == -1)
			{
				ft_putstr_fd("minishell: ", STDERR_FILENO);
				ft_putstr_fd(path, STDERR_FILENO);
				ft_putendl_fd(": Permission denied", STDERR_FILENO);
			}
			return (126);
		}
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		return (127);
	}
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putendl_fd(": command not found", STDERR_FILENO);
	return (127);
}

char	*get_path_bin(t_env *env, char *cmd)
{
	int		i;
	char	**paths;
	char	*path;
	char	*part_path;
	t_env	*env_path;

	env_path = get_env(env, "PATH");
	if (!env_path)
		return (NULL);
	paths = ft_split(env_path->value, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		if (!part_path)
		{
			free_array(paths);
			return (NULL);
		}
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (!path)
		{
			free_array(paths);
			return (NULL);
		}
		if (access(path, X_OK) == 0)
		{
			free_array(paths);
			return (path);
		}
		free(path);
		i++;
	}
	free_array(paths);
	return (NULL);
}

int	ft_execve(char *path, t_exec_cmd *cmd, t_mini *shell)
{
	int	status;

	status = 0;
	g_sig.sigchld = fork();
	if (g_sig.sigchld == -1)
		return (ERROR);
	if (g_sig.sigchld == 0)
	{
		signal(SIGPIPE, SIG_DFL);
		if (execve(path, cmd->args, shell->arr_env) == -1)
			exit(error_message(path));
	}
	else
		waitpid(g_sig.sigchld, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGPIPE)
			ft_putstr_fd(" Broken pipe\n", STDERR_FILENO);
		return (128 + WTERMSIG(status));
	}
	return (ERROR);
}

int	exec_binary(t_mini *shell, t_exec_cmd *exec)
{
	char	*path;
	int		res;

	path = get_path_bin(shell->env, exec->cmd);
	if (path)
	{
		res = ft_execve(path, exec, shell);
		free(path);
	}
	else
		res = ft_execve(exec->cmd, exec, shell);
	return (res);
	path = get_path_bin(shell->env, exec->cmd);
	if (path)
	{
		res = ft_execve(path, exec, shell);
		free(path);
	}
	else
		res = ft_execve(exec->cmd, exec, shell);
	return (res);
}

int	execute(t_mini *shell, t_exec_cmd *exec)
{
	if (!exec || !exec->cmd)
	{
		shell->exit_code = 0;
		return (0);
	}
	else if (is_builtin(exec->args[0]))
		shell->exit_code = exec_builtin(shell, exec);
	else if (exec->args[0] && exec->args[0][0])
		shell->exit_code = exec_binary(shell, exec);
	else
		shell->exit_code = 0;
	return (shell->exit_code);
}
