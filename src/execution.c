/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:12:51 by fefa              #+#    #+#             */
/*   Updated: 2025/05/04 20:13:24 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	error_message(char *path)
// {
// 	struct stat	path_stat;

// 	if (!path || ft_strlen(path) == 0)
//     {
//         ft_putstr_fd("minishell: : command not found\n", STDERR_FILENO);
//         return (127);
//     }
// 	ft_putstr_fd("minishell: ", STDERR_FILENO);
// 	ft_putstr_fd(path, STDERR_FILENO);
// 	if (stat(path, &path_stat) == -1)
// 	{
// 		if (access(path, F_OK) == -1)
// 			ft_putendl_fd(": No such file or directory", STDERR_FILENO);
// 		else
// 			ft_putendl_fd(": Permission denied", STDERR_FILENO);
// 		return (UNKNOWN_COMMAND);
// 	}
// 	if (S_ISDIR(path_stat.st_mode))
// 	{
// 		ft_putendl_fd(": is a directory", STDERR_FILENO);
// 		return (IS_DIRECTORY);
// 	}
// 	ft_putendl_fd(": command not found", STDERR_FILENO);
// 	return (UNKNOWN_COMMAND);
// }

// int error_message(char *path)
// {
//     struct stat path_stat;

//     if (!path || ft_strlen(path) == 0)
//     {
//         ft_putstr_fd("minishell: : command not found\n", STDERR_FILENO);
//         return (127);
//     }
//     ft_putstr_fd("minishell: ", STDERR_FILENO);
//     ft_putstr_fd(path, STDERR_FILENO);
    
//     // First check if file exists
//     if (access(path, F_OK) == -1)
//     {
//         ft_putendl_fd(": No such file or directory", STDERR_FILENO);
//         return (127);
//     }

//     // Then check if it's executable and get file info
//     if (stat(path, &path_stat) == -1 || access(path, X_OK) == -1)
//     {
//         ft_putendl_fd(": Permission denied", STDERR_FILENO);
//         return (126);
//     }

//     // Only check if it's a directory after confirming it exists
//     if (S_ISDIR(path_stat.st_mode))
//     {
//         ft_putendl_fd(": is a directory", STDERR_FILENO);
//         return (126);
//     }

//     // If file exists but isn't recognized as executable
//     ft_putendl_fd(": command not found", STDERR_FILENO);
//     return (127);
// }

int error_message(char *path)
{
    if (!path || ft_strlen(path) == 0)
    {
        ft_putstr_fd("minishell: : command not found\n", STDERR_FILENO);
        return (127);
    }
    ft_putstr_fd("minishell: ", STDERR_FILENO);
    ft_putstr_fd(path, STDERR_FILENO);
    ft_putendl_fd(": No such file or directory", STDERR_FILENO);
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
	int		status;

	status = 0;
	g_sig.sigchld = fork();
	if (g_sig.sigchld == -1)
		return (ERROR);
	if (g_sig.sigchld == 0)
	{
		if (execve(path, cmd->args, shell->arr_env) == -1)
		{
			//perror("execve");
			exit(error_message(path));
		}
	}
	else
		waitpid(g_sig.sigchld, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
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
}

int	execute(t_mini *shell, t_exec_cmd *exec)
{
	if (!exec || !exec->cmd || !exec->cmd[0])
	{
		shell->exit_code = 0;
		return (0);
	}
	else if (is_builtin(exec->cmd))
		shell->exit_code = exec_builtin(shell, exec);
	else
		shell->exit_code = exec_binary(shell, exec);
	return (shell->exit_code);
}
