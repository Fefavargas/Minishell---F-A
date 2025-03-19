/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_copy.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 06:43:14 by fefa              #+#    #+#             */
/*   Updated: 2025/03/18 20:50:51 by fefa             ###   ########.fr       */
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

t_env	*create_node_env(char *str)
{
	t_env	*node;
	size_t	i;
	char	**array;

	if (!(node = malloc(sizeof(t_env))))
			return (0); // ERROR!
	array = ft_split(str, '=');
	node->key = ft_strdup(array[0]);
	node->next = NULL;
	i = 0;
	while (array[++i] || i == 1)
	{
		ft_join_free(&node->value, array[i]);
		if (array[i + 1])
			ft_join_free(&node->value, "=");
	}
	free_array(array);
	return (node);
}

void	ft_copy_env(t_mini *shell, char **env)
{
	int		i;
	t_env	*node;
	t_env	*tmp;

	i = -1;
	tmp = NULL;
	while (env[++i])
	{
		node = create_node_env(env[i]);
		if (i == 0)
			shell->env = node;
		if (env[i + 1] && i != 0)
			tmp->next = node;
		tmp = node;
	}
	node->next = NULL;
}

char	*get_env(t_mini shell, char *key)
{
	t_env	*env;

	env = shell.env;
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (env->value);
		env = env->next;
	}
	return (0);
}
