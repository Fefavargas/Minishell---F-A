/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 14:35:50 by fefa              #+#    #+#             */
/*   Updated: 2025/03/28 14:18:50 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redir_in(t_mini *shell, char *file)
{
	printf("redir in fd:%d\n", shell->fdin); //DELETE LATER
	shell->fdin = open(file, O_RDONLY);
	if (shell->fdin < 0)
	{
		perror("Error opening infile");
		return ;
	}
	printf("redir in fd:%d %s\n", shell->fdin, file); //DELETE LATER
	dup2(shell->fdin, STDIN_FILENO);
	ft_close(shell->fdin);
}

void	redir_out(t_mini *shell, t_type type_token, char *file)
{
	if (type_token == TRUNC)
		shell->fdout = open(file, O_CREAT | O_WRONLY | O_TRUNC);
	else if (type_token == APPEND)
		shell->fdout = open(file, O_CREAT | O_WRONLY | O_APPEND);
	if (shell->fdout < 0)
	{
		perror("Error opening outfile");
		return ;
	}
	dup2(shell->fdout, STDOUT_FILENO);
	ft_close(shell->fdout);
}

/**
 * Function gets the previous tokes of this types  (TRUNC, APPEND, INPUT, PIPE)
 */
void	get_prev_redir(t_token **prev, t_token *token_cmd)
{
	t_token	*tmp;

	tmp = token_cmd;
	while (tmp && (tmp->type != TRUNC && tmp->type != APPEND && \
					tmp->type != INPUT && tmp->type != PIPE))
		tmp = tmp->prev;
	*prev = tmp;
}

void	redir(t_mini *shell, t_token *token_cmd)
{
	t_token	*prev;

	printf("Initializion redir\n"); //DELETE LATER
	get_prev_redir(&prev, token_cmd);
	if (prev)
		printf("prev: %s\n", prev->str); //DELETE LATER
	if (prev && prev->next && prev->next->type == ARG)
	{
		if (prev->type == INPUT)
			redir_in(shell, prev->next->str);
		else if ((prev->type == TRUNC || prev->type == APPEND))
			redir_out(shell, prev->type, prev->next->str);
	}
	else if (prev && prev->type == PIPE)
		pipex(shell);
	printf("End redir\n"); //DELETE LATER
}

