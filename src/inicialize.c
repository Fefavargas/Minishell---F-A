/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inicialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 22:35:35 by fefa              #+#    #+#             */
/*   Updated: 2025/03/18 22:52:42 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_cmd(char *input, t_mini *shell)
{
	t_cmd	*cmd;
	char	**array;
	size_t	size;
	size_t	i;

	array = ft_split_special(input, "|;");
	size = 0;
	while (array[size])
		size++;
	if (!(cmd = malloc(sizeof(t_cmd) * (size + 1))))
		return; //ERROR
	i = 0;
	while (array[i])
	{
		cmd[i].cmd = array[i];
		if (i + 1 != size)
			cmd[i].next = &cmd[i + 1];
		cmd[i].words = ft_split_special(array[i], " ");
		create_tokens(&cmd[i++]);
	}
	cmd[size - 1].next = NULL;
	shell->cmd = cmd;
}

void	inic(t_mini *shell, char **env)
{
	shell->fdin = STDIN_FILENO;
	shell->fdout = STDOUT_FILENO;
	ft_copy_env(shell, env);
	shell->exit = 0;
}
