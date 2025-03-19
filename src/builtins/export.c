/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 19:34:52 by fefa              #+#    #+#             */
/*   Updated: 2025/03/18 22:32:15 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_export(char *arg, t_env *env)
{
	t_env   *new;
	t_env	*old;

	create_node_env(&new, arg);
	if (!is_valid_env_node(*new))
		return (0);
	if ((old = get_env(env, new->key)))
		old->value = new->value;
	add_node_end(&env, new);
	return (1);
}
