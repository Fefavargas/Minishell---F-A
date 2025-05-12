/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:09:23 by fefa              #+#    #+#             */
/*   Updated: 2025/05/12 15:29:26 by fefa             ###   ########.fr       */
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

bool	exec_builtin(t_mini *shell, t_exec_cmd *cmd)
{
	bool	result;

	result = FALSE;
	if (!ft_strcmp(cmd->cmd, "echo"))
		result = ft_echo(cmd->args);
	if (!ft_strcmp(cmd->cmd, "env"))
		result = ft_env(shell->env);
	if (!ft_strcmp(cmd->cmd, "exit"))
		result = ft_exit(shell, cmd->args, cmd);
	if (!ft_strcmp(cmd->cmd, "export"))
		result = ft_export(cmd->args, shell->env, shell->secret);
	if (!ft_strcmp(cmd->cmd, "pwd"))
		result = ft_pwd();
	if (!ft_strcmp(cmd->cmd, "unset"))
		result = ft_unset(&shell->env, cmd->args);
	if (!ft_strcmp(cmd->cmd, "cd"))
		result = ft_cd(shell, cmd->args);
	return (result);
}
