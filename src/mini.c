/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvargas <fvargas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:48:31 by fefa              #+#    #+#             */
/*   Updated: 2025/05/13 18:51:11 by fvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	minishell(t_mini *shell)
{
	t_cmd	*current;
	size_t	n_pipes;

	current = shell->cmd;
	while (current && current->tokens)
	{
		n_pipes = create_pipes(current);
		create_exec_cmds(shell, current, n_pipes);
		execute(shell, current->execcmd);
		current = current->next;
	}
	reset_cmd(shell);
}

