/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albbermu <albbermu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 05:07:02 by fefa              #+#    #+#             */
/*   Updated: 2025/05/14 22:23:17 by albbermu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	create_tmp_file(int *fd)
{
	//unlink("tmp_file");
	*fd = open("tmp_file", O_CREAT | O_RDWR | O_APPEND, 0644);
	if (*fd == -1)
	{
		perror("Error creating temporary file");
		return (1);
	}
	return (0);
}

int	heredoc(t_mini *shell, t_token *token)
{
	int		fd;
	char	*str;

	(void)shell;
	if (token->type != HEREDOC || !token->next || token->next->type != DELIMITER)
		return (-1);
	create_tmp_file(&fd);
	while (g_sig.sigexit == 0)
	{
		str = readline("> ");
		if (!str)
			return (-1);
		if (ft_strcmp(str, token->next->str) == 0)
		{
			ft_close(fd);
			free(str);
			fd = open("tmp_file", O_RDONLY);
			if (fd == -1)
				return (-1);
			return (fd);
		}
		if (ft_strchr(str, '$'))
			expand_variable(&str, shell);
		ft_join_free(&str, "\n");
		write(fd, str, strlen(str));
		// ft_putstr_fd(str, fd);
		free(str);
	}
	ft_close(fd);
	return (-1);
}
