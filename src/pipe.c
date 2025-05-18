/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 17:49:36 by albermud          #+#    #+#             */
/*   Updated: 2025/05/18 17:49:55 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_pipes(char *s)
{
	size_t	count;
	size_t	i;
	char	quote;

	count = 0;
	i = 0;
	quote = 0;
	while (s[i])
	{
		if (quote == 0 && is_delimiter(s[i], "\'\""))
			quote = s[i];
		else if (quote == s[i])
			quote = 0;
		else if (!quote && s[i] == '|')
			count++;
		i++;
	}
	return (count);
}

void	create_pipes(t_cmd *cmd)
{
	size_t	n_pipes;
	size_t	i;

	n_pipes = count_pipes(cmd->cmd);
	if (!n_pipes)
		return ;
	cmd->fdpipe = malloc(sizeof(int *) * n_pipes);
	if (!cmd->fdpipe)
		return ;
	i = 0;
	while (i < n_pipes)
	{
		cmd->fdpipe[i] = malloc(sizeof(int) * 2);
		if (!cmd->fdpipe[i])
			return ;
		if (pipe(cmd->fdpipe[i]) == -1)
		{
			perror("pipe failed");
			return ;
		}
		i++;
	}
	cmd->n_pipes = n_pipes;
}

bool	find_pipe_sequence(t_cmd *cmd)
{
	t_token	*token;
	t_cmd	*current_cmd;

	current_cmd = cmd;
	while (current_cmd)
	{
		token = current_cmd->tokens;
		while (token)
		{
			get_next_type(&token, PIPE);
			if (token && token->next && token->next->type == PIPE)
				return (1);
			if (token)
				token = token->next;
		}
		current_cmd = current_cmd->next;
	}
	return (0);
}
