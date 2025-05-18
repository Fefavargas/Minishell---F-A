/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_ft2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 18:12:27 by albermud          #+#    #+#             */
/*   Updated: 2025/05/18 18:15:59 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_path_bin_iterate(char **paths, char *cmd)
{
	int		i;
	char	*path;
	int		count;

	i = 0;
	count = 0;
	while (paths[i])
	{
		path = NULL;
		ft_join_free(&path, paths[i++]);
		ft_join_free(&path, "/");
		ft_join_free(&path, cmd);
		if (path && access(path, X_OK) == 0)
			count++;
		free(path);
	}
	return (count);
}

int	count_path_bin(t_env *env, char *cmd)
{
	char	**paths;
	t_env	*env_path;
	int		count;

	env_path = get_env(env, "PATH");
	if (!env_path)
		return (0);
	paths = ft_split(env_path->value, ':');
	if (!paths)
		return (0);
	count = count_path_bin_iterate(paths, cmd);
	paths = free_array(paths);
	return (count);
}

static char	*get_path_bin_iterate(char **paths, char *cmd)
{
	int		i;
	char	*path;

	i = 0;
	while (paths[i])
	{
		path = NULL;
		ft_join_free(&path, paths[i++]);
		ft_join_free(&path, "/");
		ft_join_free(&path, cmd);
		if (path && access(path, X_OK) == 0)
			return (path);
		free(path);
	}
	return (NULL);
}

char	*get_path_bin(t_env *env, char *cmd)
{
	char	**paths;
	char	*path_result;
	t_env	*env_path;

	env_path = get_env(env, "PATH");
	if (!env_path)
		return (NULL);
	paths = ft_split(env_path->value, ':');
	if (!paths)
		return (NULL);
	path_result = get_path_bin_iterate(paths, cmd);
	paths = free_array(paths);
	return (path_result);
}
