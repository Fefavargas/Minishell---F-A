/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 19:34:52 by fefa              #+#    #+#             */
/*   Updated: 2025/05/24 13:54:07 by fefa             ###   ########.fr       */
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

bool	is_long_integer(long num)
{
	if (num > INT_MAX || num < INT_MIN)
		return (1);
	return (0);
}

bool	ft_exit(t_mini *shell, char **args)
{
	int	exit_code;

	exit_code = shell->exit_code;
	if (args && args[1])
	{
		if (!is_str_numeric(args[1]))
		{
			error_msg("exit: ", args[1], ": numeric argument required\n", 0);
			if (is_str_alpha(args[1]))
				exit_code = 2;
			else
				exit_code = 255;
		}
		else if (args[2])
			exit_code = error_msg("exit: ", "", ": too many arguments\n", 1);
		else if (is_long_integer(ft_atoi(args[1])))
			exit_code = error_msg("exit: ", "", ": it's a long-integer\n", 1);
		else
			exit_code = ft_atoi(args[1]);
	}
	free_shell(shell);
	exit(exit_code);
	return (0);
}
