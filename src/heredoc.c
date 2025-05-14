/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albbermu <albbermu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 05:07:02 by fefa              #+#    #+#             */
/*   Updated: 2025/05/13 17:39:12 by albbermu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	create_tmp_file(int *fd)
{
	*fd = open("tmp_file", O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (*fd == -1)
	{
		perror("Error creating temporary file");
		return (1);
	}
	return (0);
}

// bool	heredoc(t_mini *shell, t_token *token)
// {
// 	int		fd;
// 	char	*str;

// 	(void)shell;
// 	if (token->type != HEREDOC || !token->next || token->next->type != DELIMITER)
// 		return (1);
// 	create_tmp_file(&fd);
// 	while (g_sig.sigexit == 0)
// 	{
// 		str = readline("> ");
// 		if (!str)
// 			return (1);
// 		if (ft_strcmp(str, token->next->str) == 0)
// 		{
// 			ft_close(fd);
// 			free(str);
// 			return (0);
// 		}
// 		ft_join_free(&str, "\n");
// 		write(fd, str, strlen(str));
// 		// ft_putstr_fd(str, fd);
// 		free(str);
// 	}
// 	ft_close(fd);
// 	return (0);
// }

bool	heredoc(t_mini *shell, t_token *token)
{
	int		fd;
	char	*str;

	(void)shell;
	if (token->type != HEREDOC || !token->next || token->next->type != DELIMITER)
		return (1);
	create_tmp_file(&fd);
	while (g_sig.sigexit == 0)
	{
		str = readline("> ");
		if (!str)
			return (1);
		if (ft_strcmp(str, token->next->str) == 0)
		{
			ft_close(fd);
			free(str);
			fd = open("tmp_file", O_RDONLY);
			if (fd == -1)
				return (1);
			if (dup2(fd, STDIN_FILENO) < 0)
			{
				ft_close(fd);
				return (1);
			}
			ft_close(fd);
			return (0);
		}
		ft_join_free(&str, "\n");
		write(fd, str, strlen(str));
		// ft_putstr_fd(str, fd);
		free(str);
	}
	ft_close(fd);
	return (0);
}
