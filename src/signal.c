/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 13:22:05 by fvargas           #+#    #+#             */
/*   Updated: 2025/05/10 09:28:37 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	signal_int(int sig)
// {
// 	(void)sig;
// 	if (g_sig.sigchld == 0)
// 	{
// 		g_sig.sigexit = 1;
// 		g_sig.sigint = 1;
// 		rl_on_new_line();
// 		rl_replace_line("", 0);
// 		// ft_putstr_fd("\b\b\nminishell > ", STDERR_FILENO);
// 		// ft_putstr_fd("\n", STDERR_FILENO);
// 		rl_redisplay();
// 	}
// 	else
// 	{
// 		g_sig.sigint = 1;
// 		g_sig.sigexit = 128 + SIGINT;
// 		rl_on_new_line();
// 		rl_replace_line("", 0);
// 		ft_putstr_fd("\n", STDERR_FILENO);
// 		rl_redisplay();
// 	}
// }

// void signal_int(int sig)
// {
//     (void)sig;
//     if (g_sig.sigchld == 0)
//     {
//         g_sig.sigexit = 1;
//         g_sig.sigint = 1;
//         rl_on_new_line();
//         rl_replace_line("", 0);
//         // ft_putstr_fd("\b\b\nminishell > ", STDERR_FILENO);
//         // ft_putstr_fd("\n", STDERR_FILENO);
//         rl_redisplay();
//     }
//     else
//     {
//         g_sig.sigint = 1;
//         g_sig.sigexit = 128 + SIGINT;
//         // Remove these lines to let the child process handle the signal
//         // rl_on_new_line();
//         // rl_replace_line("", 0);
//         // ft_putstr_fd("\n", STDERR_FILENO);
//         // rl_redisplay();
//     }
// }

void signal_int(int sig)
{
    (void)sig;
    if (g_sig.sigchld == 0)  // Shell mode (no child process)
    {
        g_sig.sigexit = 1;
        g_sig.sigint = 1;
        // Clear the line
        rl_on_new_line();
        rl_replace_line("", 0);
        // Write a newline to start fresh
        write(STDERR_FILENO, "\n", 1);
        rl_redisplay();
    }
    else  // Child process is running
    {
        g_sig.sigint = 1;
        g_sig.sigexit = 128 + SIGINT;
        // Only write a newline, don't try to redisplay prompt
        write(STDERR_FILENO, "\n", 1);
        // Do NOT call rl_on_new_line, rl_replace_line or rl_redisplay here
    }
}

void	signal_quit(int sig)
{
	(void)sig;
	if (g_sig.sigchld == 0)
		ft_putstr_fd("\b\b", STDERR_FILENO);
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
	g_sig.sigchld = -1;
	g_sig.sigexit = 0;
	signal(SIGINT, signal_int);
	signal(SIGQUIT, signal_quit);
}
