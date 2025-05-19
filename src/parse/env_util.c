/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_util.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albbermu <albbermu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 21:27:34 by fefa              #+#    #+#             */
/*   Updated: 2025/05/19 18:20:34 by albbermu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_node_env(t_env	**node, char *str)
{
	t_env	*env;
	char	**array;

	*node = NULL;
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
		array = free_array(array);
		return ;
	}
	join_into_str(&env->value, &array[1], "=");
	array = free_array(array);
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

void	free_str(char *str)
{
	if (str || *str)
		free(str);
}

bool	assign_env_node(t_env *secret, char *str, bool print_error)
{
	t_env	*old;
	t_env	*new;
	char	*tmp;

	create_node_env(&new, str);
	if (!new || !is_valid_env_node(*new))
	{
		if (print_error)
			error_msg("export: '", str, "': not a valid identifier\n", 0);
		free_node(new);
		new = NULL;
		return (1);
	}
	old = get_env(secret, new->key);
	if (old)
	{
		tmp = ft_strdup(new->value);
		update_node(&old, tmp);
		free_str(tmp);
		free_node(new);
		new = NULL;
	}
	else
		add_env_end(&secret, new);
	return (0);
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
