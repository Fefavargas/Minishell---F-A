/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_free2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 07:30:55 by albermud          #+#    #+#             */
/*   Updated: 2025/05/12 15:46:57 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->str);
		free(tmp);
	}
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;

	while (cmds)
	{
		tmp = cmds;
		cmds = cmds->next;
		free(tmp->cmd);
		free_tokens(tmp->tokens);
		tmp->words = free_array(tmp->words);
		free(tmp);
	}
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}

void	free_exec_cmd(t_exec_cmd *exec)
{
	if (exec->str) //DELETE LATER
		free(exec->str); //DELETE LATER
	if (exec->cmd)
		free(exec->cmd);
	exec->args = free_array(exec->args);
}

void	free_shell(t_mini *shell, t_exec_cmd *cmd)
{
	if (cmd)
		free_exec_cmd(cmd);
	shell->arr_env = free_array(shell->arr_env);
	free_cmds(shell->cmd);
	free_env(shell->env);
	free_env(shell->secret);
	ft_close(shell->stdin);
	ft_close(shell->stdout);
	rl_clear_history();
}
