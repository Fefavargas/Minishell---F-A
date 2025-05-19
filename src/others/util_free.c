/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_free2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 07:30:55 by albermud          #+#    #+#             */
/*   Updated: 2025/05/18 20:57:39 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_node(t_env *env)
{
	if (!env)
		return ;
	if (env->key)
		free(env->key);
	if (env->value)
		free(env->value);
	free(env);
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free_node(tmp);
	}
}

void	free_exec_cmd(t_exec_cmd *exec)
{
	t_exec_cmd	*tmp;

	while (exec)
	{
		tmp = exec;
		exec = exec->next;
		if (tmp->str)
			free(tmp->str);
		if (tmp->cmd)
			free(tmp->cmd);
		tmp->args = free_array(tmp->args);
		free(tmp);
	}
}

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


