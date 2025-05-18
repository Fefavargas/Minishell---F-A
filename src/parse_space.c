/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_space.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 18:31:52 by albermud          #+#    #+#             */
/*   Updated: 2025/05/18 18:37:52 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_space_before(char **str, char *delimiters)
{
	size_t	i;
	char	quote;
	char	*s;

	i = 0;
	quote = 0;
	s = *str;
	while (s && s[i])
	{
		if (!quote && is_delimiter(s[i], "\'\""))
			quote = s[i];
		else if (quote == s[i])
			quote = 0;
		if (!quote && !is_delimiter(s[i], delimiters))
		{
			while (s && s[i] && !is_delimiter(s[i], delimiters))
			{
				if (is_redirect(&s[i++]) == 0 && is_redirect(&s[i]) != 0)
					add_string_middle(&s, " ", i);
			}
		}
		else
			i++;
	}
	*str = s;
}

static void	handle_non_delimiter_sequence(char **s, size_t *i, char *delimit)
{
	while ((*s)[*i] && !is_delimiter((*s)[*i], delimit))
	{
		if (is_redirect(&(*s)[*i]) == 1)
		{
			add_string_middle(s, " ", ++(*i));
			continue ;
		}
		(*i)++;
	}
}

void	add_space_after(char **str, char *delimit)
{
	size_t	i;
	char	quote;
	char	*s;

	i = 0;
	quote = 0;
	s = *str;
	while (s && s[i])
	{
		if (!quote && is_delimiter(s[i], "\'\""))
			quote = s[i];
		else if (quote == s[i])
			quote = 0;
		if (!quote && !is_delimiter(s[i], delimit))
			handle_non_delimiter_sequence(&s, &i, delimit);
		else
			i++;
	}
	*str = s;
}
