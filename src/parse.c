/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albbermu <albbermu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 12:25:23 by fefa              #+#    #+#             */
/*   Updated: 2025/05/09 14:51:17 by albbermu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

bool	is_open_quotes(char *line)
{
	size_t	i;
	bool	single_open;
	bool	double_open;

	i = 0;
	single_open = false;
	double_open = false;
	while (line[i])
	{
		if (!double_open && line[i] == '\'')
			single_open = !single_open;
		else if (!single_open && line[i] == '\"')
			double_open = !double_open;
		i++;
	}
	return (single_open || double_open);
}

bool	add_string_middle(char **s, char *add, int pos)
{
	char	*new_str;
	char	*str;
	int		i;
	int		k;
	int		j;

	str = *s;
	new_str = malloc(sizeof(char) * (ft_strlen(str) + ft_strlen(add) + 1));
	if (!str || !add || !new_str)
		return (1);
	i = 0;
	j = 0;
	k = 0;
	if (!str[i])
	{
		while (add[j])
			new_str[k++] = add[j++];
	}
	while (str[i])
	{
		if (i == pos)
		{
			while (add[j])
				new_str[k++] = add[j++];
		}
		new_str[k++] = str[i++];
	}
	new_str[k] = '\0';
	free(str);
	*s = new_str;
	return (0);
}

/**
 * Check if the str start with >> or << and the next char is not a delimiter, return 2
 * 		or if the str start with < or > and the next char is not a delimiter, return 1
 */
int	is_redirect(const char *str)
{
	if (!str || !str[0])
		return (0);
	if (ft_strncmp(str, "<<", 2) == 0 || ft_strncmp(str, ">>", 2) == 0)
		return (2);
	else if (is_delimiter(str[0], "<>|"))
		return (1);
	return (0);
}

bool	add_space_before(char **str, char *delimiters)
{
	size_t	i;
	char	quote;
	char	*s;
	int		redir;

	i = 0;
	quote = 0;
	s = *str;
	while (s[i])
	{
		if (!quote && is_delimiter(s[i], "\'\""))
			quote = s[i];
		else if (quote == s[i])
			quote = 0;
		if (!quote && !is_delimiter(s[i], delimiters))
		{
			while (s[i] && !is_delimiter(s[i], delimiters))
			{
				redir = is_redirect(&s[i + 1]);
				if (redir != 0 && is_redirect(&s[i]) == 0)
				{
					add_string_middle(&s, " ", i + 1);
					if (!s)
						return (0);
				}
				i++;
			}
		}
		else
			i++;
	}
	*str = s;
	return (1);
}

bool	add_space_after(char **str, char *delimiters)
{
	size_t	i;
	char	quote;
	char	*s;

	i = 0;
	quote = 0;
	s = *str;
	while (s[i])
	{
		if (!quote && is_delimiter(s[i], "\'\""))
			quote = s[i];
		else if (quote == s[i])
		{
			i++;
			quote = 0;
		}
		if (!quote && !is_delimiter(s[i], delimiters) && is_redirect(&s[i]) != 2)
		{
			if (is_redirect(&s[i]) == 1)
			{
				add_string_middle(&s, " ", ++i);
				if (!s)
					return (0);
				continue ;
			}
			while (s[i] && !is_delimiter(s[i], delimiters))
				i++;
		}
		else
			i++;
	}
	*str = s;
	return (1);
}
