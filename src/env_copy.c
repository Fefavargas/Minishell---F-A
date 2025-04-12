/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_copy.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 06:43:14 by fefa              #+#    #+#             */
/*   Updated: 2025/04/05 18:43:10 by albermud         ###   ########.fr       */
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

char	*update_node(t_env *env, char *new_value)
{
	if (env)
	{
		free(env->value);
		env->value = ft_strdup(new_value);
	}
	return (env->value);
}
