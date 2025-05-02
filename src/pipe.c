/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:09:23 by fefa              #+#    #+#             */
/*   Updated: 2025/04/30 13:32:41 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pipe(t_mini *shell)
{
	int		pipefd[2];

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
