/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvargas <fvargas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 19:34:52 by fefa              #+#    #+#             */
/*   Updated: 2025/04/17 12:52:57 by fvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	print_export_sort(t_env *secret)
{
	t_env	*cpy;
	t_env	*smallest;
	t_env	*printed;

	printed = NULL;
	while (1)
	{
		cpy = secret;
		smallest = NULL;
		while (cpy)
		{
			if ((!smallest || ft_strcmp(cpy->key, smallest->key) < 0) && \
				(!printed || ft_strcmp(printed->key, cpy->key) < 0))
				smallest = cpy;
			cpy = cpy->next;
		}
		if (!smallest)
			return (1);
		printed = smallest;
		print_export_env_node(printed);
	}
	return (0);
}

bool	ft_export_single_word(char *arg, t_env *env, t_env *secret)
{
	t_env	*new;
	t_env	*old;

	create_node_env(&new, arg);
	if (!is_valid_env_node(*new))
		return (print_export_invalid_env_node(new, arg));
	old = get_env(env, new->key);
	if (old)
	{
		update_node(old, ft_strdup(new->value));
		free_node(new);
		return (0);
	}
	add_env_end(&env, new);
	add_env_end(&secret, new);
	return (0);
}

bool	ft_export(char *args[], t_env *env, t_env *secret)
{
	size_t		i;
	bool		b;
	bool		tmp;

	i = 0;
	b = false;
	if (!args || !args[0])
		return (print_export_sort(secret));
	while (args[i])
	{
		tmp = ft_export_single_word(args[i++], env, secret);
		if (tmp == true)
			b = true;
	}
	return (b);
}
