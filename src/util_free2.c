/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_free2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 07:30:55 by albermud          #+#    #+#             */
/*   Updated: 2025/05/17 11:28:04 by albermud         ###   ########.fr       */
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
		free_exec_cmd(tmp->execcmd);
		free(tmp);
	}
}

void	free_node(t_env *env)
{
	if (!env)
		return ;
	if (env->key)
		free(env->key);
	if (env->value)
		free(env->value);
	free(env);
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free_node(tmp);
	}
}

void	free_exec_cmd(t_exec_cmd *exec)
{
	t_exec_cmd	*tmp;

	while (exec)
	{
		tmp = exec;
		exec = exec->next;
		if (tmp->str)
			free(tmp->str);
		if (tmp->cmd)
			free(tmp->cmd);
		tmp->args = free_array(tmp->args);
		free(tmp);
	}
}
