/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvargas <fvargas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 16:54:59 by fefa              #+#    #+#             */
/*   Updated: 2025/05/19 18:09:07 by fvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_delimiter(char c, const char *delimiters)
{
	size_t	i;

	i = 0;
	while (delimiters[i])
	{
		if (c == delimiters[i++])
			return (true);
	}
	return (false);
}

bool	is_blanked(char *str)
{
	while (*str)
	{
		if (*str != 32 && *str != 9)
			return (false);
		str++;
	}
	return (true);
}

const char	*get_add_str(const char *s)
{
	if (s)
		return (s);
	return ("");
}

char	*substr(const char *s, size_t start, size_t len)
{
	char	*res;
	size_t	i;

	res = malloc(len + 1);
	if (!res)
		return (NULL);
	i = 0;
	while (i < len && s[start + i])
	{
		res[i] = s[start + i];
		i++;
	}
	res[i] = '\0';
	return (res);
}
