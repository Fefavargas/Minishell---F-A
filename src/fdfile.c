/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdfile.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 13:36:37 by fvargas           #+#    #+#             */
/*   Updated: 2025/05/16 12:06:08 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	find_ampersand(char *s)
{
	size_t	i;
	char	quote;

	i = 0;
	quote = 0;
	while (s[i])
	{
		if (!quote && is_delimiter(s[i], "\'\""))
			quote = s[i];
		else if (quote == s[i])
			quote = 0;
		else if (!quote && is_delimiter(s[i], "&"))
			return (1);
		i++;
	}
	return (0);
}

// void	prepare_chld(t_mini *shell, t_exec_cmd *exec, t_cmd *cmd)
// {
// 	(void)shell;
// 	(void)cmd;
// 	dup2(exec->fdout, STDOUT_FILENO);
// 	ft_close(exec->fdout);
// 	dup2(exec->fdin, STDIN_FILENO);
// 	ft_close(exec->fdin);
// 	signal_chld();
// }

void	prepare_fd(t_exec_cmd *exec)
{
	dup2(exec->fdout, STDOUT_FILENO);
	ft_close(exec->fdout);
	dup2(exec->fdin, STDIN_FILENO);
	ft_close(exec->fdin);
}

void	prepare_chld(t_mini *shell, t_exec_cmd *exec, t_cmd *cmd)
{
	(void)shell;
	(void)cmd;
	prepare_fd(exec);
	signal_chld();
}

void	prepare_parent(int *pid, t_exec_cmd *exec)
{
	*pid = g_sig.sigchld;
	ft_close(exec->fdin);
	ft_close(exec->fdout);
}

void	close_cmd(t_cmd	*cmd)
{
	size_t	i;

	i = 0;
	while (i < cmd->n_pipes)
	{
		ft_close(cmd->fdpipe[i][0]);
		ft_close(cmd->fdpipe[i][1]);
		i++;
	}
}

void	create_array_pids(t_cmd *cmd)
{
	size_t	i;

	i = 0;
	//cmd->arr_pid = ft_calloc(sizeof(int), cmd->n_pipes + 1);
	cmd->arr_pid = ft_calloc(cmd->n_pipes + 1, sizeof(int));
	while (i < cmd->n_pipes + 1)
	{
		cmd->arr_pid[i] = 0;
		i++;
	}
}
