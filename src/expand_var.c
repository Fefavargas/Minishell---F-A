/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 14:04:07 by albbermu          #+#    #+#             */
/*   Updated: 2025/05/01 19:55:35 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *expand_variable(char *str, t_mini *shell)
{
    t_env *env_var;

    if (!str || !str[0])
        return (ft_strdup(""));
    if (str[0] == '\'' && str[ft_strlen(str) - 1] == '\'')
        return (ft_substr(str, 1, ft_strlen(str) - 2));
    if (!ft_strcmp(str, "$?"))
        return (ft_itoa(shell->exit_code));
    if (!ft_strcmp(str, "$PWD"))
    {
        char *cwd = getcwd(NULL, 0);
        return cwd ? cwd : ft_strdup("");
    }
    if (str[0] == '$' && str[1])
    {
        if (!ft_isalpha(str[1]) && str[1] != '_')
            return (ft_strdup(""));
        env_var = get_env(shell->env, &str[1]);
        if (env_var && env_var->value)
            return (ft_strdup(env_var->value));
        return (ft_strdup(""));
    }
    return (ft_strdup(str));
}

