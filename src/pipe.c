/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvargas <fvargas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:09:23 by fefa              #+#    #+#             */
/*   Updated: 2025/05/13 19:23:29 by fvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_exec_cmd_end(t_exec_cmd **first, t_exec_cmd *new)
{
	t_exec_cmd	*tmp;

	if (!new)
		return ;
	if (!(*first))
	{
		*first = new;
		return ;
	}
	tmp = *first;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

void	update_fdin_fdout(t_exec_cmd **cmd_exec, t_cmd *cmd, int i, int n_pipes)
{
	if (i != 0)
		(*cmd_exec)->fdin = cmd->fdpipe[i - 1][0];
	if (i != n_pipes)
		(*cmd_exec)->fdout = cmd->fdpipe[i][1];
}

void	create_exec_cmds(t_mini *shell, t_cmd *cmd, size_t n_pipes)
{
	t_token		*token;
	t_exec_cmd	*cmd_exec;
	size_t		i;

	token = cmd->tokens;
	i = 0;
	cmd->execcmd = NULL;
	while (i < n_pipes + 1)
	{
		cmd_exec = ft_calloc(sizeof(t_exec_cmd), 1);
		create_exec_cmd(cmd_exec, token);
		update_fdin_fdout(&cmd_exec, cmd, i++, n_pipes);
		while (token && token->type != PIPE)
		{
			redir(shell, cmd_exec, token);
			token = token->next;
		}
		add_exec_cmd_end(&cmd->execcmd, cmd_exec);
		if (token)
			token = token->next;
	}
}

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

size_t	create_pipes(t_cmd *cmd)
{
	size_t	n_pipes;
	size_t	i;

	cmd->fdpipe = NULL;
	n_pipes = count_pipes(cmd->cmd);
	if (!n_pipes)
		return (0);
	cmd->fdpipe = malloc(sizeof(int *) * n_pipes);
	if (!cmd->fdpipe)
		return (0);
	i = 0;
	while (i < n_pipes)
	{
		cmd->fdpipe[i] = malloc(sizeof(int) * 2);
		if (!cmd->fdpipe[i])
			return (0);
		if (pipe(cmd->fdpipe[i]) == -1)
		{
			perror("pipe failed");
			return (0);
		}
		i++;
	}
	return (n_pipes);
}
