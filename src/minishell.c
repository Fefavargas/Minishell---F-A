/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:48:31 by fefa              #+#    #+#             */
/*   Updated: 2025/03/21 10:53:38 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.c"

void    minishell(t_mini *shell)
{
    t_cmd   *current;

    *current = shell->cmd;
    while (!mini->exit && current)
    {
        exec_cmd(shell, current);
        reset_std(shell);
        current = current->next;
    }
}