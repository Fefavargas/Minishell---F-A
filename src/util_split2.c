/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_split2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 16:12:13 by albermud          #+#    #+#             */
/*   Updated: 2025/05/18 16:14:21 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ignore_quotes_count(char const *s, size_t *i, \
							size_t *count, bool counter)
{
	char	quote;

	if (counter)
		(*count)++;
	if (!is_delimiter(s[*i], "\'\""))
		return ;
	quote = s[*i];
	(*i)++;
	while (s[*i] && s[*i] != quote)
		(*i)++;
	if (s[*i])
		(*i)++;
}

size_t	count_regular_word(char const *s, size_t *i, char *delimiters)
{
	size_t	count;

	count = 1;
	while (s[*i] && !is_delimiter(s[*i], delimiters))
	{
		if (is_delimiter(s[*i], "\'\""))
		{
			ignore_quotes_count(s, i, &count, FALSE);
			continue ;
		}
		(*i)++;
	}
	return (count);
}
