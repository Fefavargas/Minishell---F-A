/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 16:54:59 by fefa              #+#    #+#             */
/*   Updated: 2025/05/08 10:16:49 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	count_link_list(t_token *token)
{
	t_token	*tmp;
	int		arg_count;

	arg_count = 0;
	tmp = token;
	while (tmp && tmp->type != PIPE)
	{
		if (tmp->type == ARG)
			arg_count++;
		tmp = tmp->next;
	}
	return (arg_count);
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
