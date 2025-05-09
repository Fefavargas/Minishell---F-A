/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvargas <fvargas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 19:34:52 by fefa              #+#    #+#             */
/*   Updated: 2025/05/09 18:13:21 by fvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_export_single_word(char *arg, t_env *env, t_env *secret)
{
	t_env	*new;

	assign_env_node(&new, secret, arg, true);
	if (!new)
		return (1);
	add_env_end(&env, new);
	add_env_end(&secret, new);
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
	bool	in_pipeline;

	i = 1;
	error = false;
	in_pipeline = !isatty(STDOUT_FILENO);
	if (!args || !args[1])
		return (print_export_sort(secret));
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			error = true;
		}
		else if (!in_pipeline)
		{
			tmp = ft_export_single_word(args[i], env, secret);
			if (tmp == true)
				error = true;
		}
		i++;
	}
	return (error);
}
