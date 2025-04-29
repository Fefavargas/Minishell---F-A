/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 05:07:02 by fefa              #+#    #+#             */
/*   Updated: 2025/04/29 10:00:12 by fefa             ###   ########.fr       */
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

bool	heredoc(t_mini *shell, t_token *token)
{
	int		fd;
	char	*str;

	(void)shell;
	if (token->type != HEREDOC || !token->next || token->next->type != ARG)
		return (1);
	create_tmp_file(&fd);
	while (g_sig.sigexit != 130)
	{
		str = readline("minishell > ");
		if (!str)
			return (1);
		if (ft_strcmp(str, token->next->str) == 0)
		{
			close(fd);
			free(str);
			return (0);
		}
		ft_putstr_fd(str, STDOUT_FILENO);
		free(str);
	}
	close(fd);
	return (0);
}
