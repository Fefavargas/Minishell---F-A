/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:12:51 by fefa              #+#    #+#             */
/*   Updated: 2025/05/12 15:42:29 by fefa             ###   ########.fr       */
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

char	*get_path_bin(t_env *env, char *cmd)
{
	int		i;
	char	**paths;
	char	*path;
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
		path = NULL;
		ft_join_free(&path, paths[i++]);
		ft_join_free(&path, "/");
		ft_join_free(&path, cmd);
		if (!path || access(path, X_OK) == 0)
		{
			paths = free_array(paths);
			return (path);
		}
		free(path);
	}
	paths = free_array(paths);
	return (NULL);
}

int	ft_execve(char *path, t_exec_cmd *cmd, t_mini *shell)
{
	int		status;
	// struct termios term;

    // if (isatty(STDIN_FILENO))
    //     tcgetattr(STDIN_FILENO, &term);
	status = 0;
	g_sig.sigchld = fork();
	if (g_sig.sigchld == -1)
		return (ERROR);
	if (g_sig.sigchld == 0)
	{
		//  if (isatty(STDIN_FILENO))
        //     tcsetattr(STDIN_FILENO, TCSANOW, &term);
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
	if (!path)
		return (ft_execve(exec->cmd, exec, shell));
	res = ft_execve(path, exec, shell);
	free(path);
	return (res);
}

int	execute(t_mini *shell, t_exec_cmd *exec)
{
	if (!exec || !exec->cmd)
		shell->exit_code = 0;
	else if (is_builtin(exec->args[0]))
		shell->exit_code = exec_builtin(shell, exec);
	else if (exec->args[0] && exec->args[0][0])
		shell->exit_code = exec_binary(shell, exec);
	else
		shell->exit_code = 0;
	return (shell->exit_code);
}
