/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvargas <fvargas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 19:34:52 by fefa              #+#    #+#             */
/*   Updated: 2025/04/16 21:59:05 by fvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env_node(t_env *node)
{
	if (node->key && node->value)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(node->key, STDOUT_FILENO);
		ft_putstr_fd("=\"", STDOUT_FILENO);
		ft_putstr_fd(node->value, STDOUT_FILENO);
		ft_putstr_fd("\"\n", STDOUT_FILENO);
	}
}

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
		print_env_node(printed);
	}
	return (0);
}

bool	ft_export_single_word(char *arg, t_env *env, t_env *secret)
{
	t_env	*new;
	t_env	*old;

	create_node_env(&new, arg);
	if (!is_valid_env_node(*new))
	{
		ft_putstr_fd("bash: export: `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		free_node(new);
		return (1);
	}
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
