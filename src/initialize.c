/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 22:35:35 by fefa              #+#    #+#             */
/*   Updated: 2025/05/08 09:27:54 by albermud         ###   ########.fr       */
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
	
	add_space_before(input, " ");
	add_space_after(input, " ");
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
		create_tokens(cmd, shell);
		add_cmd_end(&shell->cmd, cmd);
		free(array[i]);
		i++;
	}
	free(array);
}

void create_exec_cmd(t_exec_cmd *exec, t_token *token, t_mini *shell)
{
    exec->args = NULL;
    exec->str = NULL;
    int arg_count = 0;
    t_token *current = token;
    while (current)
    {
        if (is_redirect_type(current->type))
        {
            current = current->next;
            if (current && (current->type == FILENAME || current->type == DELIMITER))
                current = current->next;
            continue;
        }
        if (current->type != PIPE && current->str && current->str[0] != '\0')
            arg_count++;
            
        current = current->next;
    }
    exec->args = (char **)malloc(sizeof(char *) * (arg_count + 1));
    if (!exec->args)
    {
        exec->cmd = NULL;
        return;
    }
    current = token;
    int j = 0;
    while (current && j < arg_count)
    {
        if (is_redirect_type(current->type))
        {
            current = current->next;
            if (current && (current->type == FILENAME || current->type == DELIMITER))
                current = current->next;
            continue;
        }
        if (current->type != PIPE && current->str && current->str[0] != '\0')
        {
            char *unquoted_str = NULL;
            size_t len = ft_strlen(current->str);
            if (len >= 2 && current->str[0] == '\'' && current->str[len-1] == '\'')
                unquoted_str = ft_substr(current->str, 1, len - 2);
            else if (len >= 2 && current->str[0] == '\"' && current->str[len-1] == '\"')
                unquoted_str = ft_substr(current->str, 1, len - 2);
            if (unquoted_str)
                exec->args[j] = unquoted_str;
            else
            {
                exec->args[j] = ft_strdup(current->str);
                if (!exec->args[j])
                {
                    perror("ft_strdup failed");
                    free_array(exec->args);
                    exec->args = NULL;
                    exec->cmd = NULL;
                    return;
                }
            }
            j++;
        }
        
        current = current->next;
    }
    exec->args[j] = NULL;
    if (token && token->str && !ft_strcmp(token->str, "$?"))
    {
        ft_putnbr_fd(shell->exit_code, STDOUT_FILENO);
        ft_putchar_fd('\n', STDOUT_FILENO);
        exec->cmd = NULL;
        if (exec->args[0])
            free(exec->args[0]);
        exec->args[0] = ft_strdup("");
        return;
    }
    if (exec->args[0] && exec->args[0][0])
    {
        exec->cmd = ft_strdup(exec->args[0]);
        if (!exec->cmd)
        {
            perror("ft_strdup failed");
            free_array(exec->args);
            exec->args = NULL;
            return;
        }
    }
    else
        exec->cmd = NULL;
        
    if (exec->args[1])
        join_into_str(&exec->str, &exec->args[1], " ");
}

void	init(t_mini *shell, char **env)
{
	shell->stdin = dup(STDIN_FILENO);
	shell->stdout = dup(STDOUT_FILENO);
	shell->pipin = 0;
	shell->pipout = 0;
	shell->exit_code = 0;
	shell->execution = TRUE;
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
