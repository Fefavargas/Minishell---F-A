/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvargas <fvargas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:16:30 by fefa              #+#    #+#             */
/*   Updated: 2025/05/13 18:31:18 by fvargas          ###   ########.fr       */
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
		free(path_old);
		return (error_msg("cd: ", path_new, \
						": No such file or directory\n", 1));
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
		return (error_msg("cd: ", key, ": Not set\n", 1));
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
		shell->exit_code = 1;
		return (error_msg("cd: ", "", ": too many arguments\n", 1));
	}
	if (!ft_strcmp(args[1], "-"))
		return (go_key(shell->env, "OLDPWD"));
	return (go_update_oldpwd_pwd(shell->env, args[1]));
}
