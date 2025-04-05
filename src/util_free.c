/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 06:01:05 by fefa              #+#    #+#             */
/*   Updated: 2025/04/05 19:06:56 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_array(char **array)
{
	size_t	i;

	i = 0;
	while (array && array[i])
		free(array[i++]);
	free(array);
}

void	free_node(t_env *env)
{
	free(env->key);
	free(env->value);
	free(env);
}
