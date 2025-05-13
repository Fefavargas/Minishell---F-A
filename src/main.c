/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albbermu <albbermu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:09:08 by fefa              #+#    #+#             */
/*   Updated: 2025/05/13 10:59:02 by albbermu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_sig	g_sig;

void	print_type_token(t_mini shell)
{
	t_token	*token;

	token = shell.cmd->tokens;
	while (shell.cmd)
	{
		while (token)
		{
			if (token->type == PIPE)
				printf("token: %s type: PIPE\n", token->str);
			if (token->type == TRUNC)
				printf("token: %s type: TRUNC\n", token->str);
			if (token->type == INPUT)
				printf("token: %s type: INPUT\n", token->str);
			if (token->type == APPEND)
				printf("token: %s type: APPEND\n", token->str);
			if (token->type == CMD)
				printf("token: %s type: CMD\n", token->str);
			if (token->type == ARG)
				printf("token: %s type: ARG\n", token->str);
			if (token->type == HEREDOC)
				printf("token: %s type: HEREDOC\n", token->str);
			if (token->type == END)
				printf("token: %s type: END\n", token->str);
			if (token->type == FILENAME)
				printf("token: %s type: FILANAME\n", token->str);
			token = token->next;
		}
		shell.cmd = shell.cmd->next;
	}
}

void	print_all(t_mini *shell)
{
	t_cmd	*current;
	t_token	*token_current;
	int		i;
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
	print_type_token(*shell);
	//char *args[] = {"PATH", "MAIL", NULL};
	//ft_unset(&shell->env, args);
	//ft_env(shell->env); 
	//printf("PWD=%s \n", get_env(shell->env, "PWD")->value);
	// printf("HOME=%s \n", get_env(shell->env, "HOME")->value);
	// printf("%s \n", shell->arr_env[5]);
	//ft_exit(shell);
	//ft_export(NULL, shell->env, shell->secret);
}

int	main(int argc, char **argv, char **env)
{
	t_mini	shell;
	char	*input;

	(void)argv;
	(void)argc;
	init(&shell, env);
	while (!shell.exit)
	{
		init_signal();
		input = readline("minishell > ");
		if (!input)
			break ;
		if (!parse(&input, &shell))
		{
			//print_all(&shell);
			minishell(&shell);
		}
		reset_loop(&shell, input);
	}
	free_shell(&shell, NULL);
	return (0);
}
