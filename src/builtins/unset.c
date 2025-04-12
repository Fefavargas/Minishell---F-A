/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 10:05:41 by fefa              #+#    #+#             */
/*   Updated: 2025/04/12 20:10:18 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_unset(t_env **env, char *args[])
{
    t_env	*current;
    t_env	*prev;
    int		i;

    if (!env || !*env || !args)
        return (0);
    i = 0;
    while (args[i])
    {
        current = *env;
        prev = NULL;
        if (current && !ft_strcmp(current->key, args[i]))
        {
            *env = current->next;
            free_node(current);
            i++;
            continue;
        }
        while (current && ft_strcmp(current->key, args[i]))
        {
            prev = current;
            current = current->next;
        }
        if (!current)
        {
            i++;
            continue;
        }
        prev->next = current->next;
        free_node(current);
        i++;
    }
    return (0);
}
