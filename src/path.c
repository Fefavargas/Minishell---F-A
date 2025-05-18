/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 17:50:32 by fvargas           #+#    #+#             */
/*   Updated: 2025/05/18 16:46:33 by albermud         ###   ########.fr       */
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

static t_exec_cmd	*expand_command_node_with_paths(t_exec_cmd *exec_node,
												char **paths, int path_count)
{
	t_exec_cmd	*current_node_in_expansion;
	int			i;

	current_node_in_expansion = exec_node;
	free(current_node_in_expansion->cmd);
	current_node_in_expansion->cmd = ft_strdup(paths[0]);
	i = 1;
	while (i < path_count)
	{
		duplicate_exec(current_node_in_expansion);
		current_node_in_expansion = current_node_in_expansion->next;
		free(current_node_in_expansion->cmd);
		current_node_in_expansion->cmd = ft_strdup(paths[i]);
		i++;
	}
	return (current_node_in_expansion);
}

void	duplicate_path(t_env *env, t_cmd *cmd)
{
	t_exec_cmd	*current_exec;
	char		**array_path;
	int			path_count;

	current_exec = cmd->execcmd;
	while (current_exec)
	{
		path_count = count_path_bin(env, current_exec->cmd);
		if (!is_builtin(current_exec->cmd) && path_count > 1)
		{
			array_path = get_array_path(env, current_exec->cmd);
			if (array_path)
			{
				current_exec = expand_command_node_with_paths(current_exec,
						array_path, path_count);
				free_array(array_path);
			}
		}
		current_exec = current_exec->next;
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
