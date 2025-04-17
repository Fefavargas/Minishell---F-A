/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_ft_export_print.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvargas <fvargas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 22:28:05 by fvargas           #+#    #+#             */
/*   Updated: 2025/04/17 18:48:05 by fvargas          ###   ########.fr       */
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

void	print_export_env_node(t_env *node)
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

bool	print_export_invalid_env_node(char *arg)
{
	ft_putstr_fd("bash: export: '", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	return (1);
}
