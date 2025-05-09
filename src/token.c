/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albbermu <albbermu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 15:45:04 by fefa              #+#    #+#             */
/*   Updated: 2025/05/09 14:46:17 by albbermu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	double_linked_token(t_token **token)
{
	t_token	*prev;
	t_token	*tmp;

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
		return ;
	}
	tmp = *first;
	while (tmp ->next)
		tmp = tmp->next;
	tmp->next = new;
}

void	create_node_token(t_token **token, char *str)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return ;
	*token = new;
	new->str = str;
	new->next = NULL;
	new->prev = NULL;
}

void	create_tokens(t_cmd *cmd, t_mini *shell)
{
	t_token	*token;
	size_t	i;
	char	*cleaned_word;

	i = 0;
	while (cmd->words[i])
	{
		cleaned_word = ft_strdup(cmd->words[i]);
		expand_variable(&cleaned_word, shell);
		if (cleaned_word && cleaned_word[0])
		{
			create_node_token(&token, cleaned_word);
			add_token_end(&cmd->tokens, token);
		}
		i++;
	}
	double_linked_token(&cmd->tokens);
	type_tokens(&cmd->tokens);
}
