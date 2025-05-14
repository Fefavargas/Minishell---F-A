/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvargas <fvargas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 19:34:52 by fefa              #+#    #+#             */
/*   Updated: 2025/05/14 10:08:29 by fvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_export_single_word(char *arg, t_env *env, t_env *secret)
{
	assign_env_node(secret, arg, true);
	assign_env_node(env, arg, true);
	return (0);
}

bool	is_valid_identifier(char *arg)
{
	int	i;

	i = 0;
	if (!arg || !*arg || *arg == '=')
		return (false);
	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		return (false);
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

bool	ft_export(char *args[], t_env *env, t_env *secret)
{
	size_t	i;
	bool	error;
	bool	tmp;
	// bool	in_pipeline;

	i = 0;
	error = false;
	// in_pipeline = !isatty(STDOUT_FILENO);
	if (!args || !args[1])
		return (print_export_sort(secret));
	while (args[++i])
	{
		if (!is_valid_identifier(args[i]))
			return (error_msg("export: '", args[i], \
							"': not a valid identifier\n", 1));
		// else if (!in_pipeline)
		// {
		tmp = ft_export_single_word(args[i], env, secret);
		if (tmp == true)
			error = true;
		// }
	}
	return (0);
}
