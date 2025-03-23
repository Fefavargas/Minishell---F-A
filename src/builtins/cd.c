/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:16:30 by fefa              #+#    #+#             */
/*   Updated: 2025/03/23 12:38:42 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	update_oldpwd(t_env *env)
{
	t_env	*old;
	char 	*path;

	path = NULL;
	if (!getcwd(path, 0))
		return (0);
	old = get_env(env, "OLDPWD");
	if (!old)
	{
		create_node_env(&old, path);
		add_env_end(&env, old);
	}
	else
		update_node(old, path);
	return (1);
}

bool	go_homepath(t_env *env)
{
	t_env	*tmp;

	tmp = get_env(env, "HOME");
	if (!tmp)
	{
		ft_putstr_fd("bash: cd: HOME not set\n", STDERR_FILENO);
		return (0);
	}
	if (chdir(tmp->value))
		return (0);
	return (1);
}

bool	go_oldpath(t_env *env)
{
	t_env	*tmp;

	tmp = get_env(env, "OLDPWD");
	if (!tmp)
	{
		ft_putstr_fd("bash: cd: OLDPWD not set\n", STDERR_FILENO);
		return (0);
	}
	if (!update_oldpwd(env) || chdir(tmp->value))
		return (0);		
	return (1);
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
bool	ft_cd(t_env *env, char *arg)
{
	if (!arg || !arg[0])
		return (go_homepath(env));
	update_oldpwd(env);
	if (!ft_strcmp(arg, "-"))
		return (go_oldpath(env));
	if (arg[0] == '-' && arg[1])
	{
		ft_putstr_fd("bash: cd: too many arguments\n", STDERR_FILENO);
		return (0);
	}
	if (chdir(arg))
	{
		ft_putstr_fd("bash: cd: ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		return (0);
	}
	return (1);
}
