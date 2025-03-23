/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_copy.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 06:43:14 by fefa              #+#    #+#             */
/*   Updated: 2025/03/23 19:19:07 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_copy_env(t_env *env, char **env_arr)
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
