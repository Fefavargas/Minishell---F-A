/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:09:23 by fefa              #+#    #+#             */
/*   Updated: 2025/04/04 07:30:34 by albermud         ###   ########.fr       */
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
	return (FALSE);
}

bool	exec_builtin(t_mini *shell, t_exec_cmd *cmd)
{
	bool	result;

	result = FALSE;
	if (!ft_strcmp(cmd->cmd, "echo"))
		result = ft_echo(cmd->args);
	if (!ft_strcmp(cmd->cmd, "env"))
		result = ft_env(shell->env);
	if (!ft_strcmp(cmd->cmd, "exit"))
		result = ft_exit(shell, cmd->args);
	if (!ft_strcmp(cmd->cmd, "export"))
		result = ft_export(cmd->str, shell->env, shell->secret);
	if (!ft_strcmp(cmd->cmd, "pwd"))
		result = ft_pwd();
	if (!ft_strcmp(cmd->cmd, "unset"))
		result = ft_unset(shell->env, cmd->str);
	return (result);
}
