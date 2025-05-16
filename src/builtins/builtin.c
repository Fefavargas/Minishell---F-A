/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:09:23 by fefa              #+#    #+#             */
/*   Updated: 2025/05/16 16:36:24 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_builtin(char *cmd)
{
	if (!ft_strcmp(cmd, "echo"))
		return (TRUE);
	if (!ft_strcmp(cmd, "env"))
		return (TRUE);
	if (!ft_strcmp(cmd, "exit"))
		return (TRUE);
	if (!ft_strcmp(cmd, "export"))
		return (TRUE);
	if (!ft_strcmp(cmd, "pwd"))
		return (TRUE);
	if (!ft_strcmp(cmd, "unset"))
		return (TRUE);
	if (!ft_strcmp(cmd, "cd"))
		return (TRUE);
	return (FALSE);
}

bool	exec_builtin(t_mini *shell, t_exec_cmd *exec)
{
	bool	result;

	result = FALSE;
	if (!ft_strcmp(exec->cmd, "echo"))
		result = ft_echo(exec->args);
	if (!ft_strcmp(exec->cmd, "env"))
		result = ft_env(shell->env);
	if (!ft_strcmp(exec->cmd, "exit"))
		result = ft_exit(shell, exec->args);
	if (!ft_strcmp(exec->cmd, "export"))
		result = ft_export(exec->args, shell->env, shell->secret);
	if (!ft_strcmp(exec->cmd, "pwd"))
		result = ft_pwd();
	if (!ft_strcmp(exec->cmd, "unset"))
		result = ft_unset(&shell->env, exec->args);
	if (!ft_strcmp(exec->cmd, "cd"))
		result = ft_cd(shell, exec->args);
	return (result);
}
