/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 12:25:23 by fefa              #+#    #+#             */
/*   Updated: 2025/05/18 18:23:38 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

bool	find_ampersand(char *s)
{
	size_t	i;
	char	quote;

	i = 0;
	quote = 0;
	while (s[i])
	{
		if (!quote && is_delimiter(s[i], "\'\""))
			quote = s[i];
		else if (quote == s[i])
			quote = 0;
		else if (!quote && is_delimiter(s[i], "&"))
			return (1);
		i++;
	}
	return (0);
}

bool	parse(char **input, t_mini *shell)
{
	if (is_blanked(*input))
		return (1);
	if (**input)
		add_history(*input);
	if (is_open_quotes(*input))
		return (error_msg("", "", "Error syntax with open quotes\n", 1));
	if (find_ampersand(*input))
		return (error_msg("", "", "Error syntax with &\n", 1));
	add_space_before(input, " ");
	add_space_after(input, " ");
	return (create_cmd_list(*input, shell));
}
