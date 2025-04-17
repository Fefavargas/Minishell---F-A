/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvargas <fvargas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:42:31 by fvargas           #+#    #+#             */
/*   Updated: 2025/04/17 20:15:04 by fvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_valid_assignment(char *str)
{
	int	i;

	if (!str || !*str || str[0] == '=')
		return (FALSE);
	i = 0;
	while (str[++i])
	{
		if (str[i] == '=')
			return (TRUE);
	}
	return (FALSE);
}

/**
 * Trying to create the function that put new variable on the t_env secret
 * like if you run "name=Alberto" it will safe this new variable
 */
void	add_secret_env_node(t_env **secret, char *str)
{
	t_env	*new;

	assign_env_node(&new, secret, str, false);
	if (new)
		add_env_end(&secret, new);
}
