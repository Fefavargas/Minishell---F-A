/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:09:08 by fefa              #+#    #+#             */
/*   Updated: 2025/03/16 14:29:55 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*create_cmd(char *input)
{
	t_cmd	*cmd;
	char	**array;
	size_t	size;
	size_t	i;

	array = ft_split_special(input, "|;");
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
		//cmd[i].words = ft_split_special(input, " ");
		i++;
	}
	cmd[size - 1].next = NULL;
	return (cmd);
}

void	inic(t_mini *shell, char **env)
{
	shell->fdin = STDIN_FILENO;
	shell->fdout = STDOUT_FILENO;
	shell->env = env;
}

void	print_all(t_cmd *cmd)
{
	t_cmd	*current;
	int 	i;
	//int		j;

	current = cmd;
	i = 1;
	while (current)
	{
		printf("cmd[%d]: %s\n", i, current->cmd);
		//j = 0;
		i++;
		// while (current->words[j])
		// {
		// 	printf("		word[%d]: %s\n", j, current->words[j]);
		// 	j++;
		// }
		current = current->next;
	}
}

int	main(int argc, char **argv, char **env)
{
	t_mini	shell;
	char	*input;

	(void)argv;
	(void)argc;
	inic(&shell, env);
	while (1)
	{
		input = readline("minishell >");
		add_history(input);
		if (!is_open_quotes(input))
		{
			shell.cmd = create_cmd(input);
			print_all(shell.cmd);
		}
		else
			printf("Error syntax with open quotes");
	}
	return (0);
}
