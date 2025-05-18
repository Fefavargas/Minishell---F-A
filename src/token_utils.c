/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 11:19:34 by albbermu          #+#    #+#             */
/*   Updated: 2025/05/18 17:32:16 by fefa             ###   ########.fr       */
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
		else if (prev && prev->type == HEREDOC)
			token->type = DELIMITER;
		else if (prev && is_redirect_type(prev->type))
			token->type = FILENAME;
		else if (!prev || prev->type == PIPE || !find_prev_type(token, CMD))
			token->type = CMD;
		else
			token->type = ARG;
		token = token->next;
	}
}
