/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_copy.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 06:43:14 by fefa              #+#    #+#             */
/*   Updated: 2025/05/18 16:15:56 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_cpy_arr_env(char ***env_arr, char **env_arr_oficial)
{
	char	**env;
	int		size;

	size = 0;
	while (env_arr_oficial[size])
		size++;
	env = malloc(sizeof(char *) * (size + 1));
	if (!env)
		return ;
	env[size] = 0;
	while (--size >= 0)
		env[size] = ft_strdup(env_arr_oficial[size]);
	*env_arr = env;
}

void	ft_cpy_env(t_env **env, char **env_arr_oficial)
{
	size_t	i;
	t_env	*node;

	i = 0;
	while (*env_arr_oficial && env_arr_oficial[i])
	{
		node = NULL;
		create_node_env(&node, env_arr_oficial[i++]);
		if (!node)
		{
			free_env(*env);
			*env = NULL;
			return ;
		}
		add_env_end(env, node);
	}
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

bool	update_node(t_env **env, char *new_value)
{
	if (env && *env)
	{
		free((*env)->value);
		(*env)->value = ft_strdup(new_value);
		if (!(*env)->value)
			return (1);
	}
	return (0);
}

/**
 * Function to update the key and value of a node in the environment linked list
 * If the node does not exist, it creates a new node with the given key and value
 * If the path is NULL, it uses the current working directory
 */
bool	update_node_key(t_env *env, char *key, char *path)
{
	t_env	*node;

	if (!path)
		path = getcwd(NULL, 0);
	node = get_env(env, key);
	if (!node)
	{
		node = malloc(sizeof(t_env));
		if (!node)
			return (1);
		node->key = ft_strdup(key);
		node->value = ft_strdup(path);
		node->next = NULL;
		add_env_end(&env, node);
	}
	else
		update_node(&node, path);
	free(path);
	return (0);
}
