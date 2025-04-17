/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_export_print.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvargas <fvargas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 22:28:05 by fvargas           #+#    #+#             */
/*   Updated: 2025/04/16 22:31:05 by fvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

bool	print_export_invalid_env_node(t_env	*new, char *arg)
{
	ft_putstr_fd("bash: export: '", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	free_node(new);
	return (1);
}
