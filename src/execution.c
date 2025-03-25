/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:12:51 by fefa              #+#    #+#             */
/*   Updated: 2025/03/24 22:13:36 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	error_message(char *path)
{
	struct stat path_stat;

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

int		magic_box(char *path, t_exec_cmd *cmd, t_mini *shell)
{
	pid_t	pid;
	int		res;

	pid = fork();
	if (!pid) // child
	{
		execve(path, cmd->args, shell->arr_env);
		//free
		exit(error_message(path));
	}
	waitpid(pid, &res, 0);
	return (res);
}

char	*get_path(t_env *env, char *cmd)
{
	int		i;
	char	**paths;
	char	*path;
	t_env	*env_path;
	char	*part_path;

	if (!(env_path = get_env(env, "PATH")))
		return (0);
	paths = ft_split(env_path->value, ':');
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (access(path, F_OK) == 0)
		{
			free_array(paths);
			return (path);
		}
		free(path);
		i++;
	}
	free_array(paths);
	return (0);
}

int	exec_binary(t_mini *shell, t_exec_cmd *cmd)
{
	char	*path;
	int		res;

	if (!(path = get_path(shell->env, cmd->cmd)))
		res = magic_box(path, cmd, shell);
	else
		res = magic_box(cmd->cmd, cmd, shell);
	return (res);
}

bool	execute(t_mini *shell, t_exec_cmd *cmd)
{
	if (cmd)
		return (ERROR);
	if (is_builtin(cmd->cmd))
		return (exec_builtin(shell, cmd));
	else
		return (exec_binary(shell, cmd));
	return (ERROR);
}
