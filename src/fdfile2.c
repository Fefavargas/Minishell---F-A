/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdfile2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 17:04:05 by albermud          #+#    #+#             */
/*   Updated: 2025/05/18 17:04:32 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_array_pids(t_cmd *cmd)
{
	size_t		i;
	t_exec_cmd	*exec;

	i = 0;
	exec = cmd->execcmd;
	while (exec)
	{
		if (!is_builtin(exec->cmd))
			cmd->n_binary++;
		exec = exec->next;
	}
	cmd->arr_pid = ft_calloc(cmd->n_binary, sizeof(int));
	while (i < cmd->n_binary)
		cmd->arr_pid[i++] = 0;
}

void	ft_close(int fd)
{
	if (fd > 0)
		close(fd);
}
