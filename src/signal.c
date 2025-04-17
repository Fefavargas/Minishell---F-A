/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvargas <fvargas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 13:22:05 by fvargas           #+#    #+#             */
/*   Updated: 2025/04/17 17:22:10 by fvargas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signal_int(int sig)
{
	(void)sig;
	if (g_sig.sigchld == 0)
	{
		g_sig.sigexit = 1;
		g_sig.sigint = 1;
		ft_putstr_fd("\b\b\nminishell > ", STDERR_FILENO);
	}
	else
	{
		g_sig.sigint = 128 + SIGINT;
		ft_putstr_fd("\n", STDERR_FILENO);
	}
}

void	signal_quit(int sig)
{
	(void)sig;
	if (g_sig.sigchld == 0)
		ft_putstr_fd("\b\b", STDERR_FILENO);
	else
	{
		g_sig.sigquit = 128 + SIGQUIT;
		g_sig.sigexit = 1;
		ft_putstr_fd("Quit: (core dumped)\n", STDERR_FILENO);
	}
}

void	init_signal(void)
{
	g_sig.sigint = 0;
	g_sig.sigquit = 0;
	g_sig.sigchld = 0;
	g_sig.sigexit = 0;
	signal(SIGINT, signal_int);
	signal(SIGQUIT, signal_quit);
}
