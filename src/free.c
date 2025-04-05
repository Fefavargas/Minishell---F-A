/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 07:30:55 by albermud          #+#    #+#             */
/*   Updated: 2025/04/05 18:49:00 by albermud         ###   ########.fr       */
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
		free_array(tmp->words);
		free_tokens(tmp->tokens);
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
	int	i;

	i = 0;
	if (exec->args)
	{
		while (exec->args[i])
			free(exec->args[i++]);
		free(exec->args);
	}
	if (exec->cmd)
		free(exec->cmd);
}

void	free_shell(t_mini *shell)
{
	free_cmds(shell->cmd);
	free_array(shell->arr_env);
	free_env(shell->env);
	free_env(shell->secret);
}
