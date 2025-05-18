/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 14:35:50 by fefa              #+#    #+#             */
/*   Updated: 2025/05/18 17:44:44 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	create_directory(t_env *env, char *path_copy)
{
	int		status;
	pid_t	pid;
	char	*path_bin;
	char	*arr[3];

	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
	{
		arr[0] = "mkdir";
		arr[1] = path_copy;
		arr[2] = NULL;
		path_bin = get_path_bin(env, "mkdir");
		execve(path_bin, arr, NULL);
		exit(error_message(path_bin));
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
		return (0);
	return (1);
}

bool	ensure_directory_exists(t_env *env, const char *path)
{
	char		*path_copy;
	char		*last_slash;
	struct stat	st;

	path_copy = ft_strdup(path);
	if (!path_copy)
		return (1);
	last_slash = ft_strrchr(path_copy, '/');
	if (last_slash)
	{
		*last_slash = '\0';
		if (stat(path_copy, &st) != 0 || !S_ISDIR(st.st_mode))
		{
			if (create_directory(env, path_copy))
			{
				free(path_copy);
				return (1);
			}
		}
	}
	free(path_copy);
	return (0);
}

bool	redir_out(int *fdout, t_env *env, t_type type_token, char *file)
{
	int	fd;

	if (access(file, F_OK) == 0 && access(file, W_OK) < 0)
	{
		error_msg("", file, ": Permission denied\n", 1);
		return (1);
	}
	if (ensure_directory_exists(env, file))
	{
		error_msg("", file, ": Cannot create directory\n", 1);
		return (1);
	}
	if (type_token == TRUNC)
		fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (type_token == APPEND)
		fd = open(file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		fd = -1;
	if (fd < 0)
	{
		print_error("Error opening outfile", 1);
		return (1);
	}
	*fdout = fd;
	return (0);
}

bool	redir_in(int *fdin, char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		error_msg("", file, ": No such file or directory\n", 1);
		return (1);
	}
	*fdin = fd;
	return (0);
}
