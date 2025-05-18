/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_free1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 06:01:05 by fefa              #+#    #+#             */
/*   Updated: 2025/05/18 20:40:36 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Free and return NULL
 */
char	**free_array(char **array)
{
	size_t	i;

	i = 0;
	if (!array)
		return (0);
	while (array && array[i])
		free(array[i++]);
	free(array);
	return (0);
}

void	free_array_int(int **array, size_t n)
{
	size_t	i;

	i = 0;
	if (!array || n == 0)
		return ;
	while (i < n && array[i])
		free(array[i++]);
	free(array);
}

void	free_shell(t_mini *shell)
{
	reset_cmd_list(shell);
	shell->arr_env = free_array(shell->arr_env);
	free_env(shell->env);
	free_env(shell->secret);
	ft_close(shell->stdin);
	ft_close(shell->stdout);
	rl_clear_history();
}
