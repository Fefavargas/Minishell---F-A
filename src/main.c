/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:09:08 by fefa              #+#    #+#             */
/*   Updated: 2025/04/05 18:54:56 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	//ft_unset(shell->env, "PATH"); 
	//ft_env(shell->env); 
	//printf("PWD=%s \n", get_env(shell->env, "PWD")->value);
	// printf("HOME=%s \n", get_env(shell->env, "HOME")->value);
	// printf("%s \n", shell->arr_env[5]);
	//ft_exit(shell);
	//ft_export(NULL, shell->env);
}

int	main(int argc, char **argv, char **env)
{
	t_mini	shell;
	char	*input;

	(void)argv;
	(void)argc;
	init(&shell, env);
	read_history(".minishell_history");
	while (!shell.exit)
	{
		input = readline("minishell > ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		if (is_open_quotes(input))
			printf("Error syntax with open quotes\n");
		else
		{
			create_cmd(input, &shell);
			minishell(&shell);
			reset_cmd(&shell);
		}
		free(input);
	}
	write_history(".minishell_history");
	free_shell(&shell);
	return (0);
}

//next step; - create correctly the three and executad - redir, 
// know when to redir after finding CMD
			//- understand and check pipe.c
			//- add new line to secret
			//- test each builtin
