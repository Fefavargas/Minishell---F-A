/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albbermu <albbermu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 05:07:02 by fefa              #+#    #+#             */
/*   Updated: 2025/05/15 15:35:04 by albbermu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_sig.sigint = 1;
	write(STDERR_FILENO, "\n", 1);
	close(0);
}

static void	handle_null_input(int fd, t_token *token, bool is_signal)
{
	if (is_signal)
	{
		ft_close(fd);
		unlink("tmp_file");
		exit(130);
	}
	else
	{
		print_eof_warning(token->next->str);
		ft_close(fd);
		exit(0);
	}
}

static void	heredoc_child_process(t_mini *shell, t_token *token)
{
	int		fd;
	char	*str;

	signal(SIGINT, heredoc_sigint_handler);
	create_tmp_file(&fd);
	while (1)
	{
		str = readline("> ");
		if (!str)
			handle_null_input(fd, token, g_sig.sigint);
		if (ft_strcmp(str, token->next->str) == 0)
		{
			ft_close(fd);
			free(str);
			exit(0);
		}
		process_heredoc_line(fd, str, shell);
	}
}

static int	heredoc_parent_process(pid_t pid, void (*old_sigint)(int),
	t_mini *shell)
{
	int	status;
	int	fd;

	waitpid(pid, &status, 0);
	signal(SIGINT, old_sigint);
	g_sig.heredoc = 0;
	if (WIFSIGNALED(status) || (WIFEXITED(status)
			&& WEXITSTATUS(status) == 130))
	{
		g_sig.sigint = 1;
		shell->exit_code = 130;
		unlink("tmp_file");
		return (-1);
	}
	fd = open("tmp_file", O_RDONLY);
	unlink("tmp_file");
	if (fd == -1)
		return (-1);
	return (fd);
}

int	heredoc(t_mini *shell, t_token *token)
{
	void	(*old_sigint)(int);
	pid_t	pid;

	if (token->type != HEREDOC || !token->next
		|| token->next->type != DELIMITER)
		return (-1);
	old_sigint = signal(SIGINT, SIG_IGN);
	g_sig.heredoc = 1;
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
		heredoc_child_process(shell, token);
	else
		return (heredoc_parent_process(pid, old_sigint, shell));
	return (-1);
}
