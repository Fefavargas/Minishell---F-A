/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_copy.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 06:43:14 by fefa              #+#    #+#             */
/*   Updated: 2025/03/24 22:36:58 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_cpy_arr_env(char **env_arr, char **env_arr_oficial)
{
	int		size;

	size = 0;
	while (env_arr_oficial[size])
		size++;
	if (!(env_arr = malloc(sizeof(char) * (size + 1))))
		return ;
	env_arr[size] = "/0";
	while (--size >= 0)
		ft_strlcpy(env_arr[size], env_arr_oficial[size], \
			ft_strlen(env_arr_oficial[size]));
}

void	ft_cpy_env(t_env *env, char **env_arr)
{
	int		i;
	t_env	*node;

	i = -1;
	while (env_arr[++i])
	{
		create_node_env(&node, env_arr[i]);
		add_env_end(&env, node);
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
