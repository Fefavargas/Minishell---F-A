/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_copy.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 06:43:14 by fefa              #+#    #+#             */
/*   Updated: 2025/05/18 09:37:33 by albermud         ###   ########.fr       */
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
	int		i;
	t_env	*node;

	i = -1;
	while (*env_arr_oficial && env_arr_oficial[++i])
	{
		node = NULL;
		create_node_env(&node, env_arr_oficial[i]);
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

static size_t count_env_list(t_env *env_list)
{
	size_t count = 0;
	t_env *current = env_list;
	while (current)
	{
		if (current->key) // Only include if key is present
			count++;
		current = current->next;
	}
	return count;
}

char **env_list_to_array(t_env *env_list)
{
	char **arr_env;
	t_env *current;
	size_t count;
	size_t i;
	char *entry;
	size_t key_len;
	size_t value_len;
	char *ptr;

	count = count_env_list(env_list);
	arr_env = (char **)malloc(sizeof(char *) * (count + 1));
	if (!arr_env)
	{
		perror("minishell: malloc failed for env array");
		return (NULL);
	}
	current = env_list;
	i = 0;
	while (current && i < count)
	{
		if (current->key) // Only process if key exists
		{
			key_len = ft_strlen(current->key);
			value_len = 0;
			if (current->value)
				value_len = ft_strlen(current->value);
			
			entry = (char *)malloc(key_len + 1 + value_len + 1); // key + '=' + value + '\0'
			if (!entry)
			{
				perror("minishell: malloc failed for env entry string");
				while (i > 0) // Free previously allocated strings
					free(arr_env[--i]);
				free(arr_env); // Free the array itself
				return (NULL);
			}
			
			ptr = entry;
			ft_memcpy(ptr, current->key, key_len);
			ptr += key_len;
			*ptr++ = '=';
			if (current->value)
			{
				ft_memcpy(ptr, current->value, value_len);
				ptr += value_len;
			}
			*ptr = '\0';
			
			arr_env[i++] = entry;
		}
		current = current->next;
	}
	arr_env[i] = NULL; // Null-terminate the array for execve
	return (arr_env);
}
