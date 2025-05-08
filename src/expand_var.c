/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 14:04:07 by albbermu          #+#    #+#             */
/*   Updated: 2025/05/08 09:27:30 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *expand_variable(char *str, t_mini *shell)
{
    if (!str || !str[0])
        return (ft_strdup(""));
    if (str[0] == '\'' && str[ft_strlen(str) - 1] == '\'')
        return (ft_substr(str, 1, ft_strlen(str) - 2));
    
    if (!ft_strncmp(str, "$PWD", 5) || !ft_strncmp(str, "$PWD ", 5)) {
        t_env *pwd_var = get_env(shell->env, "PWD");
        if (pwd_var && pwd_var->value && ft_strchr(pwd_var->value, ' ')) {
            // Check if this is part of a cd command
            t_token *cmd_token = shell->cmd ? shell->cmd->tokens : NULL;
            if (cmd_token && cmd_token->type == CMD && !ft_strcmp(cmd_token->str, "cd")) {
                return ft_strdup("too_many_arguments");
            }
        }
    }
    
    char *result = ft_strdup("");
    char *temp;
    int i = 0;
    int start = 0;

    while (str[i])
    {
        if (str[i] == '$')
        {
            if (i > start)
            {
                char *prefix = ft_substr(str, start, i - start);
                temp = result;
                result = ft_strjoin(result, prefix);
                free(temp);
                free(prefix);
            }
            if (str[i + 1] == '?')
            {
                char *exit_str = ft_itoa(shell->exit_code);
                temp = result;
                result = ft_strjoin(result, exit_str);
                free(temp);
                free(exit_str);
                i += 2;
                start = i;
                continue;
            }
            if (ft_isalpha(str[i + 1]) || str[i + 1] == '_')
            {
                int j = i + 1;
                while (str[j] && (ft_isalnum(str[j]) || str[j] == '_'))
                    j++;
                char *var_name = ft_substr(str, i + 1, j - i - 1);
                t_env *env_var = get_env(shell->env, var_name);
                if (env_var && env_var->value)
                {
                    temp = result;
                    result = ft_strjoin(result, env_var->value);
                    free(temp);
                }
                free(var_name);
                i = j;
                start = i;
                continue;
            }
            else if (!ft_isalnum(str[i + 1]) && str[i + 1] != '_' && str[i + 1] != '?')
            {
                temp = result;
                result = ft_strjoin(result, "$");
                free(temp);
                i++;
                start = i;
                continue;
            }
        }
        i++;
    }
    if (start < i)
    {
        char *suffix = ft_substr(str, start, i - start);
        temp = result;
        result = ft_strjoin(result, suffix);
        free(temp);
        free(suffix);
    }
    return result;
}
