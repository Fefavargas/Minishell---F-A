/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 19:34:52 by fefa              #+#    #+#             */
/*   Updated: 2025/05/12 07:26:10 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_str_numeric(const char *str)
{
	if (!str || !*str)
		return (false);
	if (*str == '-' || *str == '+')
		str++;
	while (*str)
	{
		if (!isdigit(*str))
			return (false);
		str++;
	}
	return (true);
}

static bool	is_str_alpha(const char *str)
{
	if (!str || !*str)
		return (false);
	while (*str)
	{
		if (!isalpha(*str))
			return (false);
		str++;
	}
	return (true);
}

bool	ft_exit(t_mini *shell, char **args)
{
	if (args && args[1])
	{
		if (!is_str_numeric(args[1]))
		{
			error_msg("exit: ", args[1], ": numeric argument required\n", 0);
			if (is_str_alpha(args[1]))
				shell->exit_code = 2;
			else
				shell->exit_code = 255;
		}
		else if (args[2])
			return (error_msg("exit: ", "", ": too many arguments\n", 1));
		else
			shell->exit_code = ft_atoi(args[1]);
	}
	exit(shell->exit_code);
	return (0);
}
