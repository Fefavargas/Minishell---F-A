/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvargas <fvargas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 13:22:05 by fvargas           #+#    #+#             */
/*   Updated: 2025/05/15 12:10:13 by fvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signal_int(int sig)
{
	(void)sig;
	rl_on_new_line();
	rl_replace_line("", 0);
	ft_putstr_fd("\n", STDERR_FILENO);
	// rl_redisplay();
	if (g_sig.sigchld == 0)
	{
		rl_replace_line("", 0);
		g_sig.sigint = 1;
		g_sig.sigexit = 1;
		rl_forced_update_display();
	}
	else
	{
		g_sig.sigint = 1;
		g_sig.sigexit = 128 + SIGINT;
	}
}

void	signal_quit(int sig)
{
	(void)sig;
	if (g_sig.sigchld == 0)
	{
		//ft_putstr_fd("\b\b", STDERR_FILENO);
		rl_on_new_line();
		rl_redisplay();
		return ;
	}
	else
	{
		g_sig.sigquit = 1;
		g_sig.sigexit = 128 + SIGQUIT;
		ft_putstr_fd("Quit: (core dumped)\n", STDERR_FILENO);
	}
}

void	init_signal(void)
{
	g_sig.sigint = 0;
	g_sig.sigquit = 0;
	g_sig.sigchld = 0;
	g_sig.sigexit = 0;
}
