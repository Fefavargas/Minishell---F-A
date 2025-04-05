/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:16:30 by fefa              #+#    #+#             */
/*   Updated: 2025/04/05 18:38:17 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	update_oldpwd(t_env *env)
{
	t_env	*old;
	char	*path;

	path = NULL;
	if (!getcwd(path, 0))
		return (ERROR);
	old = get_env(env, "OLDPWD");
	if (!old)
	{
		create_node_env(&old, path);
		add_env_end(&env, old);
	}
	else
		update_node(old, path);
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
		return (ERROR);
	}
	if (chdir(arg))
	{
		ft_putstr_fd("bash: cd: ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		return (ERROR);
	}
	return (SUCCESS);
}
