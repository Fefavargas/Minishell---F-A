/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 15:45:04 by fefa              #+#    #+#             */
/*   Updated: 2025/03/21 11:52:26 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	type_tokens(t_token **tokens)
{
	t_token *token;

	token = *tokens;
	while (token)
	{
		if (!ft_strcmp(token->str, "|"))
			token->type = PIPE;
		else if (!ft_strcmp(token->str, ">"))
			token->type = TRUNC;
		else if (!ft_strcmp(token->str, "<"))
			token->type = INPUT;
		else if (!ft_strcmp(token->str, ">>"))
			token->type = APPEND;
		else if (!ft_strcmp(token->str, "<<"))
			token->type = HEREDOC;
		else if (!token->prev || (token->prev->type != ARG && token->prev->type != CMD))
			token->type = CMD;
		else
			token->type = ARG;
		token = token->next;
	}
}

void	double_linked_token(t_token **token)
{
	t_token	*prev;
	t_token *tmp;

	prev = NULL;
	tmp = *token;
	while (tmp)
	{
		tmp->prev = prev;
		prev = tmp;
		tmp = tmp->next;
	}
}

void	add_token_end(t_token **first, t_token *new)
{
	t_token	*tmp;

	if (!(*first))
	{
		*first = new;
		return;
	}
	tmp = *first;
	while (tmp ->next)
		tmp = tmp->next;
	tmp->next = new;
}

void	create_node_token(t_token **token, char *str)
{
	t_token	*new;

	if (!(new = malloc(sizeof(t_token))))
		return; //ERROR
	*token = new;
	new->str = str;
	new->next = NULL;
	new->prev = NULL;
}

void	create_tokens(t_cmd *cmd)
{
	t_token	*token;
	size_t	i;

	i = 0;
	while (cmd->words[i])
	{
		create_node_token(&token, cmd->words[i++]);
		add_token_end(&cmd->tokens, token);
	}
	double_linked_token(&cmd->tokens);
	type_tokens(&cmd->tokens);
}
