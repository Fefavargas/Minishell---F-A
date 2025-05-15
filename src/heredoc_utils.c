/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albbermu <albbermu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 15:33:08 by albbermu          #+#    #+#             */
/*   Updated: 2025/05/15 15:36:25 by albbermu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	create_tmp_file(int *fd)
{
	*fd = open("tmp_file", O_CREAT | O_RDWR | O_APPEND, 0644);
	if (*fd == -1)
	{
		perror("Error creating temporary file");
		return (1);
	}
	return (0);
}

void	print_eof_warning(char *delimiter)
{
	ft_putstr_fd("bash: warning: here-document delimited by ", STDERR_FILENO);
    ft_putstr_fd("end-of-file (wanted `", STDERR_FILENO);
	ft_putstr_fd(delimiter, STDERR_FILENO);
	ft_putstr_fd("')\n", STDERR_FILENO);
}

void	process_heredoc_line(int fd, char *str, t_mini *shell)
{
	if (ft_strchr(str, '$'))
		expand_variable(&str, shell);
	ft_join_free(&str, "\n");
	write(fd, str, strlen(str));
	free(str);
}
