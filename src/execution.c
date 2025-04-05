/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:12:51 by fefa              #+#    #+#             */
/*   Updated: 2025/04/05 18:47:24 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	error_message(char *path)
{
	struct stat	path_stat;

	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	if (stat(path, &path_stat) == -1)
	{
		if (access(path, F_OK) == -1)
			ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		else
			ft_putendl_fd(": Permission denied", STDERR_FILENO);
		return (UNKNOWN_COMMAND);
	}
	if (S_ISDIR(path_stat.st_mode))
	{
		ft_putendl_fd(": is a directory", STDERR_FILENO);
		return (IS_DIRECTORY);
	}
	ft_putendl_fd(": command not found", STDERR_FILENO);
	return (UNKNOWN_COMMAND);
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
	pid_t	pid;
	int		res;

	pid = fork();
	if (pid == -1)
		return (ERROR);
	if (pid == 0) // child
	{
		if (execve(path, cmd->args, shell->arr_env) == -1)
		{
			perror("execve");
			exit(error_message(path));
		}
	}
	waitpid(pid, &res, 0);
	return (res);
}

int	exec_binary(t_mini *shell, t_exec_cmd *exec)
{
	char	*path;
	int		res;

	printf("Initializion exec_binary: %s\n", exec->cmd); // DELETE LATER
	path = get_path_bin(shell->env, exec->cmd);
	if (path)
	{
		printf("found path %s\n", path); // DELETE LATER
		res = ft_execve(path, exec, shell);
		free(path);
	}
	else
	{
		printf("Didn't find path\n"); // DELETE LATER
		res = ft_execve(exec->cmd, exec, shell);
	}
	return (res);
}

int	execute(t_mini *shell, t_exec_cmd *exec)
{
	printf("Initializion execute: %s\n", exec->cmd); //DELETE LATER
	if (!exec)
		return (ERROR);
	if (is_builtin(exec->cmd))
	{
		printf("is builtin %s\n", exec->cmd); //DELETE LATER
		return (exec_builtin(shell, exec));
	}
	else
		return (exec_binary(shell, exec));
	return (ERROR);
}
