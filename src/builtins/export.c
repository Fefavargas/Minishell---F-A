/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvargas <fvargas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 19:34:52 by fefa              #+#    #+#             */
/*   Updated: 2025/04/19 18:50:09 by fvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_export_single_word(char *arg, t_env *env, t_env *secret)
{
	t_env	*new;

	assign_env_node(&new, secret, arg, true);
	if (!new)
		return (1);
	add_env_end(&env, new);
	add_env_end(&secret, new);
	return (0);
}

bool	ft_export(char *args[], t_env *env, t_env *secret)
{
	size_t		i;
	bool		b;
	bool		tmp;

	i = 1;
	b = false;
	if (!args || !args[1])
		return (print_export_sort(secret));
	while (args[i])
	{
		tmp = ft_export_single_word(args[i++], env, secret);
		if (tmp == true)
			b = true;
	}
	return (b);
}
