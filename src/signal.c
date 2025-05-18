/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 13:22:05 by fvargas           #+#    #+#             */
/*   Updated: 2025/05/18 20:48:08 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_sig.sigint = 1;
	write(STDERR_FILENO, "\n", 1);
	close(0);
}

void	signal_int(int sig)
{
	(void)sig;
	rl_on_new_line();
	rl_replace_line("", 0);
	ft_putstr_fd("\n", STDERR_FILENO);
	if (g_sig.sigchld == 0)
	{
		rl_replace_line("", 0);
		g_sig.sigint = 1;
		g_sig.sigexit = 130;
		rl_forced_update_display();
	}
	else
	{
		g_sig.sigint = 1;
		g_sig.sigexit = 130;
	}
}

void	signal_quit(int sig)
{
	(void)sig;
	if (g_sig.sigchld == 0)
	{
		rl_on_new_line();
		rl_redisplay();
		return ;
	}
	else
	{
		g_sig.sigexit = 128 + SIGQUIT;
		ft_putstr_fd("Quit: (core dumped)\n", STDERR_FILENO);
	}
}

void	init_signal(void)
{
	g_sig.sigint = 0;
	g_sig.sigchld = 0;
	g_sig.sigexit = 0;
}

void	signal_chld(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
