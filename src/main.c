/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:09:08 by fefa              #+#    #+#             */
/*   Updated: 2025/03/16 21:51:51 by fefa             ###   ########.fr       */
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
	cmd = (t_cmd *)malloc(sizeof(t_cmd) * (size + 1));
	if (!cmd)
		return; //ERROR
	i = 0;
	while (array[i])
	{
		cmd[i].cmd = array[i];
		if (i + 1 != size)
			cmd[i].next = &cmd[i + 1];
		cmd[i].words = ft_split_special(array[i], " ");
		create_tokens(&cmd[i]);
		i++;
	}
	cmd[size - 1].next = NULL;
	shell->cmd = cmd;
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
	t_token	*token_current;
	int 	i;
	int		j;

	current = cmd;
	i = 1;
	while (current)
	{
		printf("cmd[%d]: %s\n", i, current->cmd);
		j = 0;
		i++;
		while (current->words[j])
		{
			printf("		word[%d]: %s\n", j, current->words[j]);
			j++;
		}
		j = 0;
		token_current = current->tokens;
		while (token_current && j< 5)
		{
			printf("		token[%d]: %s\n", j, token_current->str);
			j++;
			token_current = token_current->next;
		}
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
		if (is_open_quotes(input))
			printf("Error syntax with open quotes");
		else
		{
			create_cmd(input, &shell);
			ft_memdel(input);
			//print_all(shell.cmd); //just to check
		}	
	}
	return (0);
}
