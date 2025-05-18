/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_util.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 16:16:30 by albermud          #+#    #+#             */
/*   Updated: 2025/05/18 19:55:52 by fefa             ###   ########.fr       */
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

bool	is_token_redir(char *s)
{
	if (!s || !s[0])
		return (false);
	if (!ft_strncmp(s, "<<", 2) || !ft_strncmp(s, ">>", 2))
		return (true);
	else if (!ft_strcmp(s, "<") || !ft_strcmp(s, ">") || !ft_strcmp(s, "|"))
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
