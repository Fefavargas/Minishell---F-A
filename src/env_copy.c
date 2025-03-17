/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_copy.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 06:43:14 by fefa              #+#    #+#             */
/*   Updated: 2025/03/17 09:46:49 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_join_free(char **s1, char *s2)
{
	char	*tmp;

	if (!*s1)
		*s1 = ft_strdup("");
	if (!s2)
		s2 = *s1;
	tmp = *s1;
	*s1 = ft_strjoin(tmp, s2);
	free(tmp);
}

void	set_value(t_env *node, char *env)
{
	char	**array;
	size_t	i;

	array = ft_split(env, '=');
	node->key = ft_strdup(array[0]);
	i = 0;
	while (array[++i] || i == 1)
	{
		ft_join_free(&node->value, array[i]);
		if (array[i + 1])
			ft_join_free(&node->value, "=");
	}
	free_array(array);
}

void	ft_copy_env(t_mini *shell, char **env)
{
	int		i;
	t_env	*node;
	t_env	*next;

	i = -1;
	if (!(node = malloc(sizeof(t_env))))
		return ; // ERROR!
	shell->env = node;
	while (env[++i])
	{
		set_value(node, env[i]);
		if (env[i + 1])
		{
			if (!(next = malloc(sizeof(t_env))))
				return; // ERROR!
			node->next = next;
			node = next;
		}
	}
	node->next = NULL;
}

char	*get_env(t_mini shell, char *key)
{
	t_env	*env;

	env = shell.env;
	while (env)
	{
		if (ft_strcmp(env->key, key))
			return (env->value);
		env = env->next;
	}
	return (0);
}
