/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvargas <fvargas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:09:23 by fefa              #+#    #+#             */
/*   Updated: 2025/04/24 19:26:35 by fvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pipe(t_mini *shell)
{
	int		pipefd[2];
	int		status;

	(void)shell;
	if (pipe(pipefd) == -1)
	{
		perror("pipe failed");
		return (ERROR);
	}
	if (fork() == 0)
	{
		ft_close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		ft_close(pipefd[0]);
		return (P_CHILD);
	}
	ft_close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	ft_close(pipefd[1]);
	waitpid(-1, &status, 0);
	return (P_PARENT);
}
