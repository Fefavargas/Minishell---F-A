/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_ft.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 21:27:34 by fefa              #+#    #+#             */
/*   Updated: 2025/03/18 22:28:06 by fefa             ###   ########.fr       */
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

void	create_node_env(t_env	**node, char *str)
{
	t_env	*env;
	size_t	i;
	char	**array;

	i = 0;
	if (!(env = malloc(sizeof(t_env))))
			return; // ERROR!
	*node = env;
	array = ft_split(str, '=');
	env->key = ft_strdup(array[0]);
	env->next = NULL;
	while (array[++i] || i == 1)
	{
		ft_join_free(&env->value, array[i]);
		if (array[i + 1])
			ft_join_free(&env->value, "=");
	}
	free_array(array);
}

bool	is_valid_env_node(t_env node)
{
	if (!node.key || !ft_strcmp(node.key, ""))
		return (0);
	if (ft_isdigit(node.key[0]))
		return (0);
	return (1);
}

void	add_env_end(t_env **env, t_env *new)
{
	t_env	*tmp;

	if (!(*env))
	{
		*env = new;
		return;
	}
	tmp = *env;
	while (tmp ->next)
		tmp = tmp->next;
	tmp->next = new;
}
