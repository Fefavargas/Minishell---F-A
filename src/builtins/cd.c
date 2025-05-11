/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:16:30 by fefa              #+#    #+#             */
/*   Updated: 2025/05/11 18:07:02 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	go_update_oldpwd_pwd(t_env *env, char *path_new)
{
	char	*path_old;

	path_old = getcwd(NULL, 0);
	if (!path_old)
		return (1);
	if (chdir(path_new) == -1)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(path_new, STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		return (1);
	}
	if (update_node_key(env, "OLDPWD", path_old) || \
		update_node_key(env, "PWD", NULL))
		return (1);
	return (0);
}

bool	go_key(t_env *env, char *key)
{
	t_env	*tmp;

	tmp = get_env(env, key);
	if (!tmp)
	{
		ft_putstr_fd("bash: cd: ", STDERR_FILENO);
		ft_putstr_fd(key, STDERR_FILENO);
		ft_putstr_fd(" not set\n", STDERR_FILENO);
		return (1);
	}
	return (go_update_oldpwd_pwd(env, tmp->value));
}

/**
	options:
	arg = -   this print OLDPWD and go there
	arg = $HOME
	arg = /usr/local
	arg = ../folder
	arg = /wrong/path - non exist directory
	arg = /root - deny permisson
*/
bool	ft_cd(t_mini *shell, char **args)
{
	if (!args || !args[0] || !args[1] || !*args[1])
		return (go_key(shell->env, "HOME"));
	if (args[2] || ft_strchr(args[1], ' '))
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	if (!ft_strcmp(args[1], "-"))
		return (go_key(shell->env, "OLDPWD"));
	return (go_update_oldpwd_pwd(shell->env, args[1]));
}
