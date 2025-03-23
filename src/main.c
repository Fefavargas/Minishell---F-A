/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:09:08 by fefa              #+#    #+#             */
/*   Updated: 2025/03/23 18:32:33 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_all(t_mini *shell)
{
	t_cmd	*current;
	t_token	*token_current;
	int 	i;
	int		j;

	current = shell->cmd;
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
		while (token_current)
		{
			printf("		token[%d]: %s\n", j, token_current->str);
			j++;
			token_current = token_current->next;
		}
		current = current->next;
	}
	//ft_unset(shell->env, "PATH"); 
	//ft_env(shell->env); 
	//printf("%s \n", get_env(shell->env, "PATH")->value);
	//ft_exit(shell);
	//ft_export(NULL, shell->env);
}

int	main(int argc, char **argv, char **env)
{
	t_mini	shell;
	char	*input;

	(void)argv;
	(void)argc;
	inic(&shell, env);
	while (!shell.exit)
	{
		input = readline("minishell >");
		add_history(input);
		if (is_open_quotes(input))
			printf("Error syntax with open quotes");
		else
		{
			create_cmd(input, &shell);
			free(input);
			print_all(&shell); //just to check
			reset(&shell);
		}
	}
	//free_array(shell->env);
	return (0);
}

//next step; - create correctly the three and executad 
