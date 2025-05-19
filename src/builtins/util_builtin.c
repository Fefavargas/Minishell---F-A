/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvargas <fvargas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 18:12:27 by albermud          #+#    #+#             */
/*   Updated: 2025/05/19 15:36:26 by fvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	assign_env_array(char ***env_arr, char *new)
{
	size_t	count;
	char	**tmp;

	count = 0;
	while ((*env_arr)[count])
		count++;
	tmp = malloc(sizeof(char *) * (count + 2));
	if (!tmp)
		return ;
	tmp[count + 1] = NULL;
	tmp[count] = ft_strdup(new);
	if (!tmp[count])
		return ;
	count = 0;
	while ((*env_arr)[count])
	{
		tmp[count] = ft_strdup((*env_arr)[count]);
		if (!tmp[count++])
		{
			free_array(tmp);
			return ;
		}
	}
	free_array(*env_arr);
	*env_arr = tmp;
}

static char	*get_path_bin_iterate(char **paths, char *cmd)
{
	size_t	i;
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

t_env	*get_env(t_env	*env, char *key)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
			return (tmp);
		tmp = tmp->next;
	}
	return (0);
}
