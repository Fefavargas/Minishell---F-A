/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 19:16:30 by fefa              #+#    #+#             */
/*   Updated: 2025/04/05 18:38:49 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_flag_n(char *s)
{
	size_t	i;

	i = 0;
	if (s && s[i] == '-')
	{
		while (s[++i])
		{
			if (s[i] != 'n')
				return (0);
		}
		return (1);
	}
	return (0);
}

/**
	-n auxiliar is to don't print a new line	

	ex: echo -n -n fernanda -n vargas;
	prompt: fernanda -n vargas%
 */
bool	ft_echo(char **args)
{
	size_t	i;
	bool	newline;

	i = 0;
	newline = 1;
	while (args[i] && check_flag_n(args[i]))
	{
		i++;
		newline = 0;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (SUCCESS);
}
