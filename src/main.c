/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:09:08 by fefa              #+#    #+#             */
/*   Updated: 2025/03/16 09:38:26 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*create_cmd(char *input)
{
	t_cmd	*cmd;
	char	**array;
	int		size;
	int		i;

	array = ft_split(input, '|');
	size = 0;
	while (array[size])
		size++;
	cmd = (t_cmd *)malloc(sizeof(t_cmd) * (size + 1));
	i = 0;
	while (array[i])
	{
		cmd[i].cmd = array[i];
		if (i + 1 != size)
			cmd[i].next = &cmd[i + 1];
		i++;
	}
	cmd[size - 1].next = NULL;
	return (cmd);
}

void	inic(t_mini *shell, char **env)
{
	shell->fdin = 0;
	shell->fdout = 1;
	shell->env = env;
}

void	print_all(t_cmd *cmd)
{
	t_cmd	*current;

	current = cmd;
	while (current)
	{
		printf("cmd[]: %s\n", current->cmd);
		current = current->next;
	}
}

int	main(int argc, char **argv, char **env)
{
	t_mini	shell;
	char	*input;

	(void)argv;
	if (argc != 1)
	{
		printf("Error: No arguments needed\n");
		return (1);
	}
	inic(&shell, env);
	while (1)
	{
		input = readline("minishell >");
		printf("%s\n", input);
		add_history(input);
		shell.cmd = create_cmd(input);
		print_all(shell.cmd);
	}
	return (0);
}