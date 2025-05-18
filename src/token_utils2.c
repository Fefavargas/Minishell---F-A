/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 16:16:30 by albermud          #+#    #+#             */
/*   Updated: 2025/05/18 16:23:11 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_next_type(t_token	**token, t_type type)
{
	while (*token)
	{
		if ((*token)->type == type)
			return ;
		*token = (*token)->next;
	}
}

static char	*init_quote_removal(char *str)
{
	char	*result;

	if (!str || !*str)
	{
		result = malloc(1);
		if (!result)
			return (NULL);
		result[0] = '\0';
		return (result);
	}
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	return (result);
}

char	*remove_quotes(char *str)
{
	size_t	i;
	size_t	j;
	char	quote;
	char	*result;

	result = init_quote_removal(str);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	quote = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '\"') && quote == 0)
			quote = str[i];
		else if (str[i] == quote)
			quote = 0;
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}
