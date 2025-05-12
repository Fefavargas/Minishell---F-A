/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 16:54:59 by fefa              #+#    #+#             */
/*   Updated: 2025/05/12 14:17:15 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Check if the str start with >> or << and the next char is not a delimiter, return 2
 * 		or if the str start with < or > and the next char is not a delimiter, return 1
 */
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

bool	is_delimiter(char c, const char *delimiters)
{
	size_t	i;

	i = 0;
	while (delimiters[i])
	{
		if (c == delimiters[i++])
			return (TRUE);
	}
	return (FALSE);
}

bool	is_blanked(char *str)
{
	while (*str)
	{
		if (*str != 32 && *str != 9)
			return (false);
		str++;
	}
	return (true);
}

void	ft_join_free(char **s1, char *s2)
{
	char	*tmp;

	if (!s1)
		return ;
	if (!*s1)
		*s1 = ft_strdup("");
	if (!s2)
		s2 = *s1;
	tmp = *s1;
	*s1 = ft_strjoin(tmp, s2);
	if (tmp)
		free(tmp);
}

int	joint_into_array_arg(char ***array, t_token *token)
{
	t_token	*tmp;
	char	**arr;
	int		i;

	arr = malloc(sizeof(char *) * (count_link_list(token) + 2));
	if (!arr)
		return (print_error("malloc failed", 1));
	arr[0] = ft_strdup(token->str);
	if (!arr[0])
		return (print_error("ft_strdup failed", 1));
	tmp = token->next;
	i = 1;
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->type == ARG)
			arr[i++] = ft_strdup(tmp->str);
		tmp = tmp->next;
	}
	arr[i] = NULL;
	*array = arr;
	return (0);
}

//DELETE LATER
void	join_into_str(char **str, char **array, char *delimitador)
{
	size_t	i;
	char	*tmp;

	i = 0;
	tmp = ft_strdup("");
	if (!tmp)
	{
		perror("ft_strdup failed");
		return ;
	}
	while (array[i])
	{
		ft_join_free(&tmp, array[i]);
		if (array[i + 1])
			ft_join_free(&tmp, delimitador);
		i++;
	}
	*str = tmp;
}
//DELETE LATER
