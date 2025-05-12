/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 12:25:23 by fefa              #+#    #+#             */
/*   Updated: 2025/05/12 18:49:21 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

	i = 0;
	j = 0;
	k = 0;
	str = *s;
	new_str = malloc(sizeof(char) * (ft_strlen(str) + ft_strlen(add) + 1));
	if (!str || !add || !new_str)
		return (1);
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
		{
			while (s && s[i] && !is_delimiter(s[i], delimit))
			{
				if (is_redirect(&s[i]) == 1)
				{
					add_string_middle(&s, " ", ++i);
					continue ;
				}
				i++;
			}
		}
		else
			i++;
	}
	*str = s;
}

void	parse(char **input, t_mini *shell)
{
	
	add_space_before(input, " ");
	add_space_after(input, " ");
	create_cmd(*input, shell);
}
