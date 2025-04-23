/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvargas <fvargas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 16:54:59 by fefa              #+#    #+#             */
/*   Updated: 2025/04/23 17:44:48 by fvargas          ###   ########.fr       */
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
	while (tmp)
	{
		if (tmp->type == ARG)
			arg_count++;
		else if (is_redirect(tmp->type))
			tmp = tmp->next; // Skip the filename
		tmp = tmp->next;
	}
	return (arg_count);
}

void	joint_into_array_arg(char ***array, t_token *token, t_mini *shell)
{
	t_token	*tmp;
	char	**arr;
	int		i;

	arr = malloc(sizeof(char *) * (count_link_list(token) + 2));
	if (!arr)
	{
		perror("malloc failed");
		return ;
	}
	arr[0] = ft_strdup(token->str);
	if (!arr[0])
	{
		perror("ft_strdup failed");
		return ;
	}
	tmp = token->next;
	i = 1;
	while (tmp)
	{
		if (tmp->type == ARG)
		{
			arr[i] = expand_variable(tmp->str, shell);
			if (!arr[i])
			{
				perror("expand_variable failed");
				free_array(arr);
				arr = NULL;
				return ;
			}
			i++;
		}
		else if (is_redirect(tmp->type))
			tmp = tmp->next; // Skip the filename
		tmp = tmp->next;
	}
	arr[i] = NULL;
	*array = arr;
}
