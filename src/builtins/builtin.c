/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvargas <fvargas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:09:23 by fefa              #+#    #+#             */
/*   Updated: 2025/05/19 18:08:57 by fvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_builtin(char *cmd)
{
	if (!ft_strcmp(cmd, "echo"))
		return (true);
	if (!ft_strcmp(cmd, "env"))
		return (true);
	if (!ft_strcmp(cmd, "exit"))
		return (true);
	if (!ft_strcmp(cmd, "export"))
		return (true);
	if (!ft_strcmp(cmd, "pwd"))
		return (true);
	if (!ft_strcmp(cmd, "unset"))
		return (true);
	if (!ft_strcmp(cmd, "cd"))
		return (true);
	return (false);
}

bool	exec_builtin(t_mini *shell, t_exec_cmd *exec)
{
	bool	result;

	duplicate_fd(exec);
	result = false;
	if (!ft_strcmp(exec->cmd, "echo"))
		result = ft_echo(exec->args);
	if (!ft_strcmp(exec->cmd, "env"))
		result = ft_env(shell->env);
	if (!ft_strcmp(exec->cmd, "exit"))
		result = ft_exit(shell, exec->args);
	if (!ft_strcmp(exec->cmd, "export"))
		result = ft_export(exec->args, shell->env, \
							shell->secret, &shell->arr_env);
	if (!ft_strcmp(exec->cmd, "pwd"))
		result = ft_pwd();
	if (!ft_strcmp(exec->cmd, "unset"))
		result = ft_unset(&shell->env, exec->args);
	if (!ft_strcmp(exec->cmd, "cd"))
		result = ft_cd(shell, exec->args);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	return (result);
}
