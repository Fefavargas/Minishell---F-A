/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_ft.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 21:27:34 by fefa              #+#    #+#             */
/*   Updated: 2025/05/08 09:26:52 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_node_env(t_env	**node, char *str)
{
	t_env	*env;
	char	**array;

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
		free_array(array);
		return ;
	}
	join_into_str(&env->value, &array[1], "=");
	free_array(array);
}

void	assign_env_node(t_env **new, t_env *secret, char *str, bool print_error)
{
	t_env	*old;

	create_node_env(new, str);
	if (!is_valid_env_node(**new))
	{
		if (print_error)
			print_export_invalid_env_node(str);
		//free_node((*new));
		*new = NULL;
		return ;
	}
	old = get_env(secret, (*new)->key);
	if (old)
	{
		update_node(old, ft_strdup((*new)->value));
		//free_node((*new));
		*new = NULL;
	}
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
