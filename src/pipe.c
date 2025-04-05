/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:09:23 by fefa              #+#    #+#             */
/*   Updated: 2025/04/05 19:02:03 by albermud         ###   ########.fr       */
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
	if (shell->pid == P_CHILD)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		//shell->pipout = pipefd[1];
		close(pipefd[1]);
		return (P_CHILD);
	}
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	//shell->pipin = pipefd[0];
	//close(pipefd[0]);
	return (P_PARENT);
}
