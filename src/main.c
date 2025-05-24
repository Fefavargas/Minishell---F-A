/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:09:08 by fefa              #+#    #+#             */
/*   Updated: 2025/05/24 14:48:01 by fefa             ###   ########.fr       */
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
		init_signal(&shell);
		input = readline("minishell > ");
		if (!input)
			break ;
		if (!parse(&input, &shell))
			minishell(&shell);
		reset_loop(&shell, &input);
	}
	free_shell(&shell);
	return (0);
}
