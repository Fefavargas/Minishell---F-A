/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_copy.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 06:43:14 by fefa              #+#    #+#             */
/*   Updated: 2025/03/21 07:46:29 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_copy_env(t_mini *shell, char **env)
{
	int		i;
	t_env	*node;

	i = -1;
	while (env[++i])
	{
		create_node_env(&node, env[i]);
		add_env_end(&shell->env, node);
	}
}

t_env	*get_env(t_env	*env, char *key)
{
	// t_env	*env;

	// env = shell.env;
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (env);
		env = env->next;
	}
	return (0);
}
