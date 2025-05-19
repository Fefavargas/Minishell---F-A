/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdfile.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 17:49:24 by albermud          #+#    #+#             */
/*   Updated: 2025/05/18 21:16:31 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_close(int fd)
{
	if (fd > 0)
		close(fd);
}

bool	create_tmp_file(int *fd)
{
	*fd = open("tmp_file", O_CREAT | O_RDWR | O_APPEND, 0644);
	if (*fd == -1)
	{
		perror("Error creating temporary file");
		return (1);
	}
	return (0);
}

void	duplicate_fd(t_exec_cmd *exec)
{
	dup2(exec->fdout, STDOUT_FILENO);
	ft_close(exec->fdout);
	dup2(exec->fdin, STDIN_FILENO);
	ft_close(exec->fdin);
}

void	update_fdin_fdout(t_exec_cmd **exec, t_cmd *cmd, int i, int n_pipes)
{
	if (i != 0)
		(*exec)->fdin = cmd->fdpipe[i - 1][0];
	if (i != n_pipes)
		(*exec)->fdout = cmd->fdpipe[i][1];
}

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
