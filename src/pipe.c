/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:09:23 by fefa              #+#    #+#             */
/*   Updated: 2025/03/21 16:43:16 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pipex(t_mini *shell)
{
	int		pipefd[2];

	if (pipe(pipefd) == -1)
		return (-1);
	if ((shell->pid = fork()) == -1)
		return (-1);
	if (shell->pid == 0) //child
	{
		dup2(pipefd[1], STDOUT_FILENO);
		shell->pipout = pipefd[1];
		close(pipefd[0]);
		close(pipefd[1]);
		return (P_CHILD);
	}
	dup2(pipefd[0], STDIN_FILENO);
	shell->pipin = pipefd[0];
	close(pipefd[0]);
	close(pipefd[1]);
	return (P_PARENT);
}