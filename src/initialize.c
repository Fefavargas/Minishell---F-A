/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albbermu <albbermu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 22:35:35 by fefa              #+#    #+#             */
/*   Updated: 2025/05/07 14:26:45 by albbermu         ###   ########.fr       */
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
		create_tokens(cmd, shell);
		add_cmd_end(&shell->cmd, cmd);
		free(array[i]);
		i++;
	}
	free(array);
}

// void	create_exec_cmd(t_exec_cmd *exec, t_token *token, t_mini *shell)
// {
// 	exec->args = NULL;
// 	exec->cmd = ft_strdup(token->str);
// 	if (!exec->cmd)
// 	{
// 		perror("ft_strdup failed");
// 		return ;
// 	}
// 	joint_into_array_arg(&exec->args, token, shell);
// 	join_into_str(&exec->str, &exec->args[1], " ");
// 	// DELETE LATER -it shouldn't be here
// 	if (!ft_strcmp(token->str, "$?"))
// 	{
// 		ft_putnbr_fd(shell->exit_code, STDOUT_FILENO);
// 		ft_putchar_fd('\n', STDOUT_FILENO);
// 		exec->cmd = NULL;
// 		exec->args[0] = ft_strdup("");
// 	}
// 	if (!exec->cmd)
// 	{
// 		if (ft_strcmp(token->str, "$?") != 0)
//             perror("expand_variable failed");
// 		free_array(exec->args);
// 		exec->args = NULL;
// 		return ;
// 	}
// }

// void create_exec_cmd(t_exec_cmd *exec, t_token *token, t_mini *shell)
// {
//     exec->args = NULL;
//     exec->str = NULL;
//     joint_into_array_arg(&exec->args, token, shell);
//     if (!exec->args)
//     {
//         exec->cmd = NULL;
//         return;
//     }
//     if (!ft_strcmp(token->str, "$?"))
//     {
//         ft_putnbr_fd(shell->exit_code, STDOUT_FILENO);
//         ft_putchar_fd('\n', STDOUT_FILENO);
//         exec->cmd = NULL;
//         exec->args[0] = ft_strdup("");
//         return;
//     }
//     int i = 0;
//     int j = 0;
//     char **new_args = exec->args;

//     while (new_args[i] && !new_args[i][0])
//         i++;
//     while (new_args[i])
//     {
//         new_args[j] = new_args[i];
//         i++;
//         j++;
//     }
//     new_args[j] = NULL;
//     if (exec->args[0] && exec->args[0][0])
//     {
//         exec->cmd = ft_strdup(exec->args[0]);
//         if (!exec->cmd)
//         {
//             perror("ft_strdup failed");
//             free_array(exec->args);
//             exec->args = NULL;
//             return;
//         }
//     }
//     else
//         exec->cmd = NULL;
//     if (exec->args[1])
//         join_into_str(&exec->str, &exec->args[1], " ");
// }

// void create_exec_cmd(t_exec_cmd *exec, t_token *token, t_mini *shell)
// {
//     exec->args = NULL;
//     exec->str = NULL;
    
//     // First, we need to count valid arguments (skip redirection operators and targets)
//     int arg_count = 0;
//     t_token *current = token;
    
//     while (current)
//     {
//         // Skip redirection operators and their targets
//         if (is_redirect_type(current->type))
//         {
//             current = current->next;  // Skip the redirection operator
//             if (current && (current->type == FILENAME || current->type == DELIMITER))
//                 current = current->next;  // Skip the filename/delimiter
//             continue;
//         }
        
//         // Only count tokens that should be arguments
//         if (current->type != PIPE && current->str && current->str[0] != '\0')
//             arg_count++;
            
//         current = current->next;
//     }
    
//     // Allocate space for arguments (plus NULL terminator)
//     exec->args = (char **)malloc(sizeof(char *) * (arg_count + 1));
//     if (!exec->args)
//     {
//         exec->cmd = NULL;
//         return;
//     }
    
//     // Fill arguments array with only valid argument tokens
//     current = token;
//     int j = 0;
    
//     while (current && j < arg_count)
//     {
//         // Skip redirection operators and their targets
//         if (is_redirect_type(current->type))
//         {
//             current = current->next;  // Skip the redirection operator
//             if (current && (current->type == FILENAME || current->type == DELIMITER))
//                 current = current->next;  // Skip the filename/delimiter
//             continue;
//         }
        
//         // Add only valid arguments
//         if (current->type != PIPE && current->str && current->str[0] != '\0')
//         {
//             exec->args[j] = ft_strdup(current->str);
//             if (!exec->args[j])
//             {
//                 perror("ft_strdup failed");
//                 free_array(exec->args);
//                 exec->args = NULL;
//                 exec->cmd = NULL;
//                 return;
//             }
//             j++;
//         }
        
//         current = current->next;
//     }
//     exec->args[j] = NULL;
    
//     // Special handling for $?
//     if (token && token->str && !ft_strcmp(token->str, "$?"))
//     {
//         ft_putnbr_fd(shell->exit_code, STDOUT_FILENO);
//         ft_putchar_fd('\n', STDOUT_FILENO);
//         exec->cmd = NULL;
//         if (exec->args[0])
//             free(exec->args[0]);
//         exec->args[0] = ft_strdup("");
//         return;
//     }
    
//     // Set command name and argument string
//     if (exec->args[0] && exec->args[0][0])
//     {
//         exec->cmd = ft_strdup(exec->args[0]);
//         if (!exec->cmd)
//         {
//             perror("ft_strdup failed");
//             free_array(exec->args);
//             exec->args = NULL;
//             return;
//         }
//     }
//     else
//         exec->cmd = NULL;
        
//     if (exec->args[1])
//         join_into_str(&exec->str, &exec->args[1], " ");
// }

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
