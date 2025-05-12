/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 11:19:34 by albbermu          #+#    #+#             */
/*   Updated: 2025/05/12 18:39:39 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_redirect_type(t_type type)
{
	if (type == TRUNC || type == APPEND || type == INPUT || type == HEREDOC)
		return (true);
	return (false);
}

bool	is_token_redir(char *s)
{
	if (!s || !s[0])
		return (false);
	if (!ft_strncmp(s, "<<", 2) || !ft_strncmp(s, ">>", 2))
		return (true);
	else if (!ft_strcmp(s, "<") || !ft_strcmp(s, ">") || !ft_strcmp(s, "|"))
		return (true);
	return (false);
}

t_type	type_redirect(char *str)
{
	if (!ft_strcmp(str, "|"))
		return (PIPE);
	else if (!ft_strcmp(str, "<"))
		return (INPUT);
	else if (!ft_strcmp(str, ">"))
		return (TRUNC);
	else if (!ft_strcmp(str, ">>"))
		return (APPEND);
	else if (!ft_strcmp(str, "<<"))
		return (HEREDOC);
	return (0);
}

void	type_tokens(t_token **tokens)
{
	t_token	*token;
	t_token	*prev;

	token = *tokens;
	while (token)
	{
		prev = token->prev;
		if (is_token_redir(token->str))
			token->type = type_redirect(token->str);
		else if (!prev || prev->type == PIPE)
			token->type = CMD;
		else if (prev && prev->type == HEREDOC)
			token->type = DELIMITER;
		else if (prev && is_redirect_type(prev->type))
			token->type = FILENAME;
		else
			token->type = ARG;
		token = token->next;
	}
}

char	*remove_quotes(char *str)
{
	size_t	i;
	size_t	j;
	char	quote;
	char	*result;

	i = 0;
	j = 0;
	quote = 0;
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '\"') && quote == '\0')
			quote = str[i];
		else if (str[i] == quote)
			quote = '\0';
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}
