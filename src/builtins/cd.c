/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:16:30 by fefa              #+#    #+#             */
/*   Updated: 2025/05/02 07:48:22 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	update_oldpwd(t_env *env)
{
	t_env	*old;
	char	*path;

	path = getcwd(NULL, 0);  // ✅ Correct dynamic allocation
	if (!path)
		return (ERROR);
	old = get_env(env, "OLDPWD");
	if (!old)
	{
		create_node_env(&old, path);
		add_env_end(&env, old);
	}
	else
		update_node(old, path);
	free(path);
	return (SUCCESS);
}

bool	go_homepath(t_env *env)
{
	t_env	*tmp;

	tmp = get_env(env, "HOME");
	if (!tmp)
	{
		ft_putstr_fd("bash: cd: HOME not set\n", STDERR_FILENO);
		return (ERROR);
	}
	if (chdir(tmp->value))
		return (ERROR);
	return (SUCCESS);
}

bool	go_oldpath(t_env *env)
{
	t_env	*tmp;

	tmp = get_env(env, "OLDPWD");
	if (!tmp)
	{
		ft_putstr_fd("bash: cd: OLDPWD not set\n", STDERR_FILENO);
		return (ERROR);
	}
	if (!update_oldpwd(env) || chdir(tmp->value))
		return (ERROR);
	return (SUCCESS);
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

bool ft_cd(t_mini *shell, char **args)
{
	t_env   *pwd;
	char    *path;
	if (!args || !args[0])
		return go_homepath(shell->env);
	if (args[2])
	{
		ft_putstr_fd("bash: cd: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	if (!args[1])
		return go_homepath(shell->env);
	if (!*args[1])
		return go_homepath(shell->env);
	if (!ft_strcmp(args[1], "-"))
		return go_oldpath(shell->env);
	if (chdir(args[1]) == -1)
	{
		ft_putstr_fd("bash: cd: ", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		return (1);
	}
	if (!update_oldpwd(shell->env))
		return (1);
	pwd = get_env(shell->env, "PWD");
	if (pwd)
	{
		if (!(path = getcwd(NULL, 0)))
		{
			ft_putstr_fd("bash: cd: error retrieving current directory\n", STDERR_FILENO);
			return (1);
		}
		update_node(pwd, path);
		free(path);
	}
	return (0);
}
