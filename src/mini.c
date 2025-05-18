/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:48:31 by fefa              #+#    #+#             */
/*   Updated: 2025/05/18 18:19:15 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_fdin_fdout(t_exec_cmd **exec, t_cmd *cmd, int i, int n_pipes)
{
	if (i != 0)
		(*exec)->fdin = cmd->fdpipe[i - 1][0];
	if (i != n_pipes)
		(*exec)->fdout = cmd->fdpipe[i][1];
}

void	minishell(t_mini *shell)
{
	t_cmd	*current;

	current = shell->cmd;
	while (current)
	{
		if (!current->tokens)
			shell->exit_code = 0;
		else
		{
			create_pipes(current);
			create_exec_cmds(shell, current);
			create_array_pids(current);
			execute(shell, current);
		}
		current = current->next;
	}
	reset_cmd(shell);
}
