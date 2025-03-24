/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:09:23 by fefa              #+#    #+#             */
/*   Updated: 2025/03/23 22:37:59 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_builtin(char *cmd)
{
	if(!ft_strcmp(cmd, "echo"))
		return (TRUE);
	if(!ft_strcmp(cmd, "env"))
		return (TRUE);
	if(!ft_strcmp(cmd, "exit"))
		return (TRUE);
	if(!ft_strcmp(cmd, "export"))
		return (TRUE);
	if(!ft_strcmp(cmd, "pwd"))
		return (TRUE);
	if(!ft_strcmp(cmd, "unset"))
		return (TRUE);
	return (FALSE);
}

bool	exec_builtin(t_mini *shell, char **args)
{
	bool	result;
	
	result = FALSE;
	if(!ft_strcmp(args[0], "echo"))
		result = ft_echo(args);
	if(!ft_strcmp(args[0], "env"))
		result = ft_env(shell->env);
	if(!ft_strcmp(args[0], "exit"))
		result = ft_exit(shell);
	if(!ft_strcmp(args[0], "export"))
		result = ft_export(args[1], shell->env, shell->secret);
	if(!ft_strcmp(args[0], "pwd"))
		result = ft_pwd();
	if(!ft_strcmp(args[0], "unset"))
		result = ft_unset(shell->env, args[1]);
	return result;
}
