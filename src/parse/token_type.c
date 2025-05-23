/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 11:19:34 by albbermu          #+#    #+#             */
/*   Updated: 2025/05/18 20:02:32 by fefa             ###   ########.fr       */
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

bool	find_prev_type(t_token *token, t_type type)
{
	t_token	*prev;

	prev = token;
	while (prev)
	{
		if (prev->type == type)
			return (1);
		prev = prev->prev;
	}
	return (0);
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
