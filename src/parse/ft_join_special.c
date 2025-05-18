/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_join.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 15:47:55 by albbermu          #+#    #+#             */
/*   Updated: 2025/05/18 20:31:28 by fefa             ###   ########.fr       */
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

char	*strjoin_three(const char *s1, const char *s2, const char *s3)
{
	char	*res;
	size_t	len;
	size_t	i;
	size_t	j;

	if (!s1 || !s2 || !s3)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3);
	res = malloc(len + 1);
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[j])
		res[i++] = s1[j++];
	j = 0;
	while (s2[j])
		res[i++] = s2[j++];
	j = 0;
	while (s3[j])
		res[i++] = s3[j++];
	res[i] = '\0';
	return (res);
}
