/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_util.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 16:16:30 by albermud          #+#    #+#             */
/*   Updated: 2025/05/18 21:33:41 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirect(const char *str)
{
	if (!str || !str[0])
		return (0);
	if (ft_strncmp(str, "<<", 2) == 0 || ft_strncmp(str, ">>", 2) == 0)
		return (2);
	else if (is_delimiter(str[0], "<>|"))
		return (1);
	return (0);
}

bool	is_redirect_type(t_type type)
{
	if (type == TRUNC || type == APPEND || type == INPUT || type == HEREDOC)
		return (true);
	return (false);
}

t_type	type_redirect(char *str)
{
	if (!ft_strcmp(str, "|"))
		return (PIPE);
	else if (!ft_strcmp(str, "<"))
		return (INPUT);
	else if (!ft_strcmp(str, ">"))
		return (TRUNC);
	else if (!ft_strcmp(str, ">>"))
		return (APPEND);
	else if (!ft_strcmp(str, "<<"))
		return (HEREDOC);
	return (0);
}

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
