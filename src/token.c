/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 15:45:04 by fefa              #+#    #+#             */
/*   Updated: 2025/03/16 19:03:13 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_token_end(t_token *token, t_token *new)
{
	t_token	*tmp;

	if (!token)
	{
		token = new;
		return;
	}
	tmp = token;
	while (tmp ->next)
		tmp = tmp->next;
	tmp->next = new;
}

void	init_token(t_token *token, char *str)
{
	token->str = str;
	if (!ft_strncmp(token->str, ">", 1))
		token->type = TRUNC;
	else if (!ft_strncmp(token->str, "<" , 1))
		token->type = INPUT;
	else if (!ft_strncmp(token->str, ">>", 2))
		token->type = APPEND;
	else if (!ft_strncmp(token->str, "<<", 2))
		token->type = HEREDOC;
	else
		token->type = ARG;
	token->next = NULL;
}

void	create_node_token(t_token **token, char *str)
{
	t_token	*new;

	if (!(new = malloc(sizeof(t_token))))
		return; //ERROR
	*token = new;
	init_token(new, str);
}

void	create_tokens(t_cmd *cmd)
{
	t_token	*token;
	size_t	i;

	i = 0;
	while (cmd->words[i])
	{
		create_node_token(&token, cmd->words[i++]);
		add_token_end(cmd->tokens, token);
	}
}
