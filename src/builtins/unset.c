/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 10:05:41 by fefa              #+#    #+#             */
/*   Updated: 2025/03/17 10:35:27 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_unset(t_env *env, char *unset)
{
	t_env	*current;
	t_env	*tmp;

	current = env;
	if (current && !ft_strcmp(current->key, unset))
	{
		tmp = current->next;
		env = tmp;
		free(current);
		return (SUCCESS);
	}
	while (current && ft_strcmp(current->key, unset))
	{
		tmp = current;
		current = current->next;
		if (!current)
			return (SUCCESS);
	}
	tmp->next = current->next;
	free(current);
	return (SUCCESS);
}
