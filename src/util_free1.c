/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_free1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 06:01:05 by fefa              #+#    #+#             */
/*   Updated: 2025/05/10 08:26:04 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**free_array(char **array)
{
	size_t	i;

	i = 0;
	while (array && array[i])
		free(array[i++]);
	free(array);
	return (0);
}

void	free_node(t_env *env)
{
	if (!env)
		return ;
	free(env->key);
	free(env->value);
	free(env);
}

int	print_error(char *str, int num)
{
	perror(str);
	return (num);
}

// static void	free_memory(size_t item_count, char **dest)
// {
// 	while (item_count > 0)
// 	{
// 		item_count--;
// 		free(*(dest + item_count));
// 	}
// 	free(dest);
// }
