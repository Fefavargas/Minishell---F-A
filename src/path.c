/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 17:50:32 by fvargas           #+#    #+#             */
/*   Updated: 2025/05/16 16:42:18 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	duplicate_exec(t_exec_cmd *exec)
{
	t_exec_cmd	*exec_copy;

	exec_copy = NULL;
	exec_copy = malloc(sizeof(t_exec_cmd *));
	if (!exec_copy)
		return ;
	exec_copy->cmd = ft_strdup(exec->cmd);
	exec_copy->str = ft_strdup(exec->str);
	exec_copy->fdin = exec->fdin;
	exec_copy->fdout = exec->fdout;
	exec_copy->execution = exec->execution;
	exec_copy->next = exec->next;
	exec_copy->args = exec->args;
	exec->next = exec_copy;
}

void	duplicate_path(t_env *env, t_cmd *cmd)
{
	char		**array_path;
	t_exec_cmd	*exec;
	int			i;
	int			count;

	exec = cmd->execcmd;
	i = 0;
	while (exec)
	{
		count = count_path_bin(env, exec->cmd);
		if (!is_builtin(exec->cmd) && count > 1)
		{
			while (i++ < count - 1)
			{
				duplicate_exec(exec);
				exec = exec->next;
			}
			array_path = get_array_path(env, exec->cmd);
			i = 0;
			while (i < count - 1)
			{
				free(exec->cmd);
				exec->cmd = ft_strdup(array_path[i++]);
			}
		}
		exec = exec->next;
	}
}

void	get_array_path_bin(char ***array, t_env *env, char *cmd, int size)
{
	int		i;
	int		j;
	char	**paths;
	char	*path;
	t_env	*env_path;

	env_path = get_env(env, "PATH");
	if (!env_path)
		return ;
	paths = ft_split(env_path->value, ':');
	if (!paths)
		return ;
	i = 0;
	j = 0;
	while (paths[i] && j < size)
	{
		path = NULL;
		ft_join_free(&path, paths[i++]);
		ft_join_free(&path, "/");
		ft_join_free(&path, cmd);
		if (!path || access(path, X_OK) == 0)
			*array[j++] = ft_strdup(path);
		free(path);
	}
	paths = free_array(paths);
}

char	**get_array_path(t_env *env, char *cmd)
{
	int		count;
	char	**array;

	count = count_path_bin(env, cmd);
	if (!count)
		return (0);
	array = malloc(sizeof(char *) * (count + 1));
	if (!array)
		return (0);
	array[count] = 0;
	get_array_path_bin(&array, env, cmd, count);
	return (array);
}
