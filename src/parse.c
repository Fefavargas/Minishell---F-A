/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 12:25:23 by fefa              #+#    #+#             */
/*   Updated: 2025/03/16 12:25:36 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_open_quotes(char *line)
{
	size_t	i;
	int		open;

	i = 0;
	open = 0;
	while (line[i])
	{
		if (open == 0 && line[i] == '\'')
			open = 1;
		if (open == 0 && line[i] == '\"')
			open = 2;
		if (open == 1 && line[i] == '\"')
			open = 0;
		if (open == 2 && line[i] == '\"')
			open = 0;
		i++;
	}
	if (open)
		return (1);
	return (0); 
}