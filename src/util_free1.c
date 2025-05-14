/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_free1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvargas <fvargas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 06:01:05 by fefa              #+#    #+#             */
/*   Updated: 2025/05/14 11:57:57 by fvargas          ###   ########.fr       */
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
	while (i < n)
		free(array[i++]);
	free(array);
}

void	free_shell(t_mini *shell, t_exec_cmd *cmd)
{
	if (cmd)
		free_exec_cmd(cmd);
	shell->arr_env = free_array(shell->arr_env);
	free_cmds(shell->cmd);
	free_env(shell->env);
	free_env(shell->secret);
	ft_close(shell->stdin);
	ft_close(shell->stdout);
	rl_clear_history();
}

int	print_error(char *str, int ret)
{
	perror(str);
	return (ret);
}

int	error_msg(char *str1, char *str2, char *str3, int ret)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(str1, STDERR_FILENO);
	ft_putstr_fd(str2, STDERR_FILENO);
	ft_putstr_fd(str3, STDERR_FILENO);
	return (ret);
}