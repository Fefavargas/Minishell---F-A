/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 17:50:41 by albermud          #+#    #+#             */
/*   Updated: 2025/05/18 16:05:04 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_cmd_end(t_cmd **first, t_cmd *new)
{
	t_cmd	*tmp;

	if (!(*first))
	{
		*first = new;
		return ;
	}
	tmp = *first;
	while (tmp ->next)
		tmp = tmp->next;
	tmp->next = new;
}

void	create_node_cmd(t_cmd **cmd, char *str)
{
	t_cmd	*new;

	new = malloc(sizeof(t_cmd));
	if (!new)
	{
		perror("malloc failed");
		return ;
	}
	*cmd = new;
	new->cmd = str;
	new->words = ft_split_special(str, " ");
	new->next = NULL;
	new->tokens = NULL;
}

bool	create_cmd(t_mini *shell, char *array)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (print_error("malloc failed", 1));
	cmd->cmd = ft_strdup(array);
	if (!cmd->cmd)
		return (print_error("ft_strdup failed", 1));
	cmd->words = ft_split_special(array, " ");
	cmd->next = NULL;
	cmd->tokens = NULL;
	cmd->execcmd = NULL;
	cmd->fdpipe = NULL;
	cmd->arr_pid = NULL;
	cmd->n_pipes = 0;
	cmd->n_binary = 0;
	create_tokens(cmd, shell);
	add_cmd_end(&shell->cmd, cmd);
	return (0);
}

bool	create_cmd_list(char *input, t_mini *shell)
{
	char	**array;
	size_t	i;

	i = 0;
	array = ft_split_special(input, ";");
	if (!array)
		return (print_error("split failed", 1));
	while (array[i])
	{
		if (create_cmd(shell, array[i++]))
		{
			free_array(array);
			return (error_msg("error by creating command\n", "", "", 1));
		}
	}
	if (find_pipe_sequence(shell->cmd))
	{
		free_array(array);
		return (error_msg("Error syntax with |\n", "", "", 1));
	}
	array = free_array(array);
	return (0);
}

void	init(t_mini *shell, char **env)
{
	shell->stdin = dup(STDIN_FILENO);
	shell->stdout = dup(STDOUT_FILENO);
	shell->exit_code = 0;
	shell->cmd = NULL;
	shell->arr_env = NULL;
	shell->env = NULL;
	shell->secret = NULL;
	ft_cpy_arr_env(&shell->arr_env, env);
	ft_cpy_env(&shell->env, shell->arr_env);
	ft_cpy_env(&shell->secret, shell->arr_env);
	signal(SIGINT, signal_int);
	signal(SIGQUIT, SIG_IGN);
}
