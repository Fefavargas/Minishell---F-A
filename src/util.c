/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 16:54:59 by fefa              #+#    #+#             */
/*   Updated: 2025/04/09 11:40:55 by albermud         ###   ########.fr       */
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
	tmp = NULL;
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
	int		i;

	i = 0;
	tmp = token;
	while (tmp && tmp->type == ARG)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

void	joint_into_array_arg(char ***array, t_token *token)
{
	t_token	*tmp;
	char	**arr;
	int		i;

	arr = malloc(sizeof(char *) * (count_link_list(token) + 1));
	if (!arr)
		return ;
	i = 0;
	tmp = token;
	while (tmp && tmp->type == ARG)
	{
		arr[i] = ft_strdup(tmp->str);
		if (!arr[i])
		{
			free_array(arr);
			return ;
		}
		i++;
		tmp = tmp->next;
	}
	arr[i] = NULL;
	*array = arr;
}
