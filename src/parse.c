/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 12:25:23 by fefa              #+#    #+#             */
/*   Updated: 2025/05/04 15:25:12 by fefa             ###   ########.fr       */
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
	if (!str || !add)
		return (1);
	new_str = malloc(sizeof(char) * (ft_strlen(str) + ft_strlen(add) + 1));
	if (!new_str)
		return (1);
	i = 0;
	j = 0;
	k = 0;
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

int	is_redirect_without_space(const char *str, char *delimiters)
{
	if (ft_strncmp(str, "<<", 2) == 0 || ft_strncmp(str, ">>", 2) == 0)
	{
		if (!is_delimiter(str[2], delimiters))
			return (2);
	}
	else if (is_delimiter(str[0], "<>|"))
	{
		if (!is_delimiter(str[1], delimiters))
			return (1);
	}
	return (0);
}

bool	add_space_redirection(char **str, char *delimiters)
{
	size_t	i;
	int		redirect;
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
			quote = 0;
		if (!quote && !is_delimiter(s[i], delimiters))
		{
			redirect = is_redirect_without_space(&s[i], delimiters);
			if (redirect)
			{
				i += redirect;
				add_string_middle(&s, " ", i);
				if (!s)
					return (0);				
			}
 			while (s[i] && (quote || !is_delimiter(s[i], delimiters)))
			{
				if (!quote && is_delimiter(s[i], "\'\""))
					quote = s[i];
				else if (quote == s[i])
					quote = 0;
				i++;
			}
		}
		else
			i++;
	}
	*str = s;
	return (1);
}
