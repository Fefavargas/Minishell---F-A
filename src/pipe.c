/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:09:23 by fefa              #+#    #+#             */
/*   Updated: 2025/05/08 09:25:02 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * If there is already one rediretion to sdtout, it shouldn't open the pipes.
 */
int	ft_pipe(t_mini *shell)
{
    int		pipefd[2];

	if (shell->fdout != -1)
		return (ERROR);
	if (pipe(pipefd) == -1)
	{
		perror("pipe failed");
		return (ERROR);
	}
	shell->pipin = pipefd[0];
	shell->pipout = pipefd[1];
	dup2(shell->pipout, STDOUT_FILENO);
	close(shell->pipout);
	return (1);
}
