/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 22:35:35 by fefa              #+#    #+#             */
/*   Updated: 2025/04/05 15:25:02 by albermud         ###   ########.fr       */
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

	if (!(new = malloc(sizeof(t_cmd))))
		return ; //ERROR
	*cmd = new;
	new->cmd = str;
	new->words = ft_split_special(str, " ");
	new->next = NULL;
	new->tokens = NULL;
	create_tokens(new);
}

void	create_cmd(char *input, t_mini *shell)
{
	t_cmd	*cmd;
	char	**array;
	size_t	i;

	i = 0;
	array = ft_split_special(input, ";");
	while (array[i])
	{
		create_node_cmd(&cmd, array[i++]);
		add_cmd_end(&shell->cmd, cmd);
	}
}

void create_exec_cmd(t_exec_cmd *exec, t_token *token)
{
    t_token *tmp;
    int arg_count = 0;

    // Skip the command token
    tmp = token->next;

    // Count the number of arguments
    while (tmp && (tmp->type == ARG))
    {
        arg_count++;
        tmp = tmp->next;
    }

    // Allocate memory for arguments
    exec->args = malloc(sizeof(char *) * (arg_count + 1));
    if (!exec->args)
        return;

    // Populate the arguments array
    tmp = token->next;
    arg_count = 0;
    while (tmp && (tmp->type == ARG))
    {
        exec->args[arg_count++] = ft_strdup(tmp->str);
        tmp = tmp->next;
    }
    exec->args[arg_count] = NULL;

    // Set the command
    exec->cmd = ft_strdup(token->str);

    // Debug print
    printf("create_exec_cmd: cmd='%s', args[0]='%s'\n", exec->cmd, exec->args[0]);
}


void	init(t_mini *shell, char **env)
{
	shell->fdin = STDIN_FILENO;
	shell->fdout = STDOUT_FILENO;
	shell->cmd = NULL;
	shell->arr_env = NULL;
	shell->env = NULL;
	shell->secret = NULL;
	ft_cpy_arr_env(&shell->arr_env, env);
	ft_cpy_env(&shell->env, shell->arr_env);
	ft_cpy_env(&shell->secret, shell->arr_env);
	reset_fds(shell, 0);
	(void)env;
	shell->exit = 0;
}

