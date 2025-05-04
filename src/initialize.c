/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 22:35:35 by fefa              #+#    #+#             */
/*   Updated: 2025/05/04 15:27:55 by fefa             ###   ########.fr       */
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

void	create_cmd(char **input, t_mini *shell)
{
	t_cmd	*cmd;
	char	**array;
	size_t	i;

	i = 0;
	add_space_redirection(input, " ");
	array = ft_split_special(*input, ";");
	if (!array)
		return ;
	while (array[i])
	{
		cmd = malloc(sizeof(t_cmd));
		if (!cmd)
		{
			perror("malloc failed");
			free_array(array);
			return ;
		}
		cmd->cmd = ft_strdup(array[i]);
		if (!cmd->cmd)
		{
			perror("ft_strdup failed");
			free(cmd);
			free_array(array);
			return ;
		}
		cmd->words = ft_split_special(array[i], " ");
		cmd->next = NULL;
		cmd->tokens = NULL;
		create_tokens(cmd);
		add_cmd_end(&shell->cmd, cmd);
		free(array[i]);
		i++;
	}
	free(array);
}

void	create_exec_cmd(t_exec_cmd *exec, t_token *token, t_mini *shell)
{
	exec->args = NULL;
		exec->cmd = ft_strdup(token->str);
	if (!exec->cmd)
	{
		perror("ft_strdup failed");
		return ;
	}
	joint_into_array_arg(&exec->args, token, shell);
	join_into_str(&exec->str, &exec->args[1], " ");
	// DELETE LATER -it shouldn't be here
	if (!ft_strcmp(token->str, "$?"))
	{
		ft_putnbr_fd(shell->exit_code, STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		exec->cmd = NULL;
		exec->args[0] = ft_strdup("");
	}
	if (!exec->cmd)
	{
		if (ft_strcmp(token->str, "$?") != 0)
            perror("expand_variable failed");
		free_array(exec->args);
		exec->args = NULL;
		return ;
	}
}


void	init(t_mini *shell, char **env)
{
	// dup2(STDIN_FILENO, shell->stdin);
	// dup2(STDOUT_FILENO, shell->stdout);
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
	reset_fds(shell, 0);
	shell->exit = 0;
}
