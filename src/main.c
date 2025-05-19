/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:09:08 by fefa              #+#    #+#             */
/*   Updated: 2025/05/19 06:53:39 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_sig	g_sig;

int	main(int argc, char **argv, char **env)
{
	t_mini	shell;
	char	*input;

	(void)argv;
	(void)argc;
	init(&shell, env);
	while (1)
	{
		init_signal();
		input = readline("minishell > ");
		if (!input)
			break ;
		if (g_sig.sigint)
		{
			shell.exit_code = g_sig.sigexit;
			g_sig.sigint = 0;
		}
		if (!parse(&input, &shell))
			minishell(&shell);
		reset_loop(&shell, &input);
	}
	free_shell(&shell);
	return (shell.exit_code);
}
