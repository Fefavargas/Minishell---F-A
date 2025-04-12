/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 11:19:34 by albbermu          #+#    #+#             */
/*   Updated: 2025/04/06 15:52:50 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_quotes(char *str)
{
	size_t	i;
	size_t	j;
	char	quote;
	char	*result;

	i = 0;
	j = 0;
	quote = '\0';
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '\"') && quote == '\0')
			quote = str[i];
		else if (str[i] == quote)
			quote = '\0';
		else if (str[i] == '\\' && str[i + 1] && (str[i + 1] == '\'' || str[i + 1] == '\"'))
			result[j++] = str[++i];
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}
