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

void	inic_token(t_token *token, char *str)
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
	ft_strlen(str);
}

void	create_tokens(t_cmd *cmd)
{
	t_token	*token;
	size_t	i;

	token = malloc(sizeof(t_token));
	if (!token)
		return; //ERROR
	cmd->tokens = token;
	i = 0;
	while (cmd->words[i])
	{
		inic_token(token, cmd->words[i]);
		if (cmd->words[i + 1])
		{
			token->next = malloc(sizeof(t_token));
			if (!token->next)
				return; //ERROR	
			token = token->next;
		}
		i++;
	}
	token->next = NULL;
}
