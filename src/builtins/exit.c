/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvargas <fvargas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 19:34:52 by fefa              #+#    #+#             */
/*   Updated: 2025/04/17 17:18:30 by fvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_numeric(const char *str)
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
	if (args && args[0])
	{
		if (!is_numeric(args[0]))
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(args[0], STDERR_FILENO);
			ft_putendl_fd(": numeric argument required", STDERR_FILENO);
			if (is_str_alpha(args[0]))
				shell->exit_code = 2;
			else
				shell->exit_code = 255;
		}
		else if (args[1])
		{
			ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
			return (1);
		}
		else
			shell->exit_code = ft_atoi(args[0]);
	}
	shell->exit = true;
	exit(shell->exit_code);
	return (0);
}
