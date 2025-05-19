/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 15:47:02 by fefa              #+#    #+#             */
/*   Updated: 2025/05/18 16:00:23 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_token_type_line(t_token *token)
{
	if (token->type == PIPE)
		printf("token: %s type: PIPE\n", token->str);
	else if (token->type == TRUNC)
		printf("token: %s type: TRUNC\n", token->str);
	else if (token->type == INPUT)
		printf("token: %s type: INPUT\n", token->str);
	else if (token->type == APPEND)
		printf("token: %s type: APPEND\n", token->str);
	else if (token->type == CMD)
		printf("token: %s type: CMD\n", token->str);
	else if (token->type == ARG)
		printf("token: %s type: ARG\n", token->str);
	else if (token->type == HEREDOC)
		printf("token: %s type: HEREDOC\n", token->str);
	else if (token->type == END)
		printf("token: %s type: END\n", token->str);
	else if (token->type == FILENAME)
		printf("token: %s type: FILANAME\n", token->str);
}

void	print_type_token(t_mini shell)
{
	t_token	*token;

	while (shell.cmd)
	{
		token = shell.cmd->tokens;
		while (token)
		{
			print_token_type_line(token);
			token = token->next;
		}
		shell.cmd = shell.cmd->next;
	}
}

static void	print_cmd_info(t_cmd *cmd, int i)
{
	int		j;
	t_token	*token_current;

	printf("cmd[%d]: %s\n", i, cmd->cmd);
	j = 0;
	while (cmd->words[j])
	{
		printf("		word[%d]: %s\n", j, cmd->words[j]);
		j++;
	}
	j = 0;
	token_current = cmd->tokens;
	while (token_current)
	{
		printf("		token[%d]: %s\n", j, token_current->str);
		j++;
		token_current = token_current->next;
	}
}

void	print_all(t_mini *shell)
{
	t_cmd	*current;
	int		i;

	current = shell->cmd;
	i = 1;
	while (current)
	{
		print_cmd_info(current, i);
		i++;
		current = current->next;
	}
	print_type_token(*shell);
}
