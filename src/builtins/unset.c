/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albbermu <albbermu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 10:05:41 by fefa              #+#    #+#             */
/*   Updated: 2025/05/09 14:56:28 by albbermu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_unset_single_word(t_env **env, char *arg)
{
	t_env	*current;
	t_env	*prev;

	current = *env;
	if (!env || !*env || !arg)
		return (0);
	if (current && !ft_strcmp(current->key, arg))
	{
		*env = current->next;
		free(current);
		return (SUCCESS);
	}
	while (current && ft_strcmp(current->key, arg))
	{
		prev = current;
		current = current->next;
		if (!current)
			return (SUCCESS);
	}
	prev->next = current->next;
	free(current);
	return (SUCCESS);
}

int	ft_unset(t_env **env, char **args)
{
	int	i;

	if (!args || !*args)
		return (0);
	i = 1;
	while (args[i])
		ft_unset_single_word(env, args[i++]);
	return (0);
}
