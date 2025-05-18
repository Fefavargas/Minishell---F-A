/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 16:16:30 by albermud          #+#    #+#             */
/*   Updated: 2025/05/18 17:36:06 by fefa             ###   ########.fr       */
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