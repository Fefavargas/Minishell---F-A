/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_ft.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albbermu <albbermu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 21:27:34 by fefa              #+#    #+#             */
/*   Updated: 2025/05/15 15:38:57 by albbermu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_node_env(t_env	**node, char *str)
{
	t_env	*env;
	char	**array;

	*node = NULL;
	env = malloc(sizeof(t_env));
	if (!env)
		return ;
	env->key = NULL;
	env->value = NULL;
	env->next = NULL;
	*node = env;
	array = ft_split(str, '=');
	if (!array || !array[0])
		return ;
	env->key = ft_strdup(array[0]);
	if (!env->key)
	{
		array = free_array(array);
		return ;
	}
	join_into_str(&env->value, &array[1], "=");
	array = free_array(array);
}

void	assign_env_node(t_env *secret, char *str, bool print_error)
{
	t_env	*old;
	t_env	*new;

	create_node_env(&new, str);
	if (!new || !is_valid_env_node(*new))
	{
		if (print_error)
			error_msg("export: '", str, "': not a valid identifier\n", 0);
		free_node(new);
		new = NULL;
		return ;
	}
	old = get_env(secret, new->key);
	if (old)
	{
		update_node(old, ft_strdup(new->value));
		free_node(new);
		new = NULL;
	}
	else
		add_env_end(&secret, new);
}

bool	is_valid_env_node(t_env node)
{
	int	i;

	if (!node.key || (!ft_isalpha(node.key[0]) && node.key[0] != '_'))
		return (false);
	i = 1;
	while (node.key[i])
	{
		if (!ft_isalnum(node.key[i]) && node.key[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

void	add_env_end(t_env **first, t_env *new)
{
	t_env	*tmp;

	if (!new)
		return ;
	if (!(*first))
	{
		*first = new;
		return ;
	}
	tmp = *first;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

int	count_path_bin(t_env *env, char *cmd)
{
	int		i;
	char	**paths;
	char	*path;
	t_env	*env_path;
	int		count;

	env_path = get_env(env, "PATH");
	if (!env_path)
		return (0);
	paths = ft_split(env_path->value, ':');
	if (!paths)
		return (0);
	i = 0;
	count = 0;
	while (paths[i])
	{
		path = NULL;
		ft_join_free(&path, paths[i++]);
		ft_join_free(&path, "/");
		ft_join_free(&path, cmd);
		if (!path && access(path, X_OK) == 0)
			count++;
		free(path);
	}
	paths = free_array(paths);
	return (count);
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

char	*get_array_path_bin(char ***array, t_env *env, char *cmd, int size)
{
	int		i;
	int		j;
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
	j = 0;
	while (paths[i] && j < size)
	{
		path = NULL;
		ft_join_free(&path, paths[i++]);
		ft_join_free(&path, "/");
		ft_join_free(&path, cmd);
		if (!path || access(path, X_OK) == 0)
			array[i][j++] = ft_strdup(path);
		free(path);
	}
	paths = free_array(paths);
	return (NULL);
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
