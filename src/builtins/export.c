/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albbermu <albbermu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 19:34:52 by fefa              #+#    #+#             */
/*   Updated: 2025/05/19 18:17:38 by albbermu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_export_env_node(t_env *node)
{
	if (node->key && node->value)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(node->key, STDOUT_FILENO);
		ft_putstr_fd("=\"", STDOUT_FILENO);
		ft_putstr_fd(node->value, STDOUT_FILENO);
		ft_putstr_fd("\"\n", STDOUT_FILENO);
	}
}

bool	print_export_sort(t_env *secret)
{
	t_env	*cpy;
	t_env	*smallest;
	t_env	*printed;

	printed = NULL;
	while (1)
	{
		cpy = secret;
		smallest = NULL;
		while (cpy)
		{
			if ((!smallest || ft_strcmp(cpy->key, smallest->key) < 0) && \
				(!printed || ft_strcmp(printed->key, cpy->key) < 0))
				smallest = cpy;
			cpy = cpy->next;
		}
		if (!smallest)
			return (1);
		printed = smallest;
		print_export_env_node(printed);
	}
	return (0);
}

int	is_valid_identifier(char *str)
{
	char	**array;
	char	*valid;
	int		result;

	array = ft_split(str, '=');
	if (!array || !array[0] || !ft_isalpha(str[0]) || ft_strchr(array[0], '-'))
	{
		free_array(array);
		return (0);
	}
	valid = ft_strchr(str, '=');
	if (valid == 0)
		result = 1;
	else
		result = 2;
	free_array(array);
	return (result);
}

bool	ft_export(char *args[], t_env *env, t_env *secret, char ***env_arr)
{
	size_t	i;
	bool	error;
	int		ret;

	i = 0;
	error = 0;
	if (!args || !args[1])
		return (print_export_sort(secret));
	while (args[++i])
	{
		ret = is_valid_identifier(args[i]);
		if (ret == 0)
			error = error_msg("export: '", args[i],
					"': not a valid identifier\n", 1);
		else
		{
			assign_env_node(secret, args[i], true);
			assign_env_array(env_arr, args[i]);
			if (ret == 2)
				assign_env_node(env, args[i], true);
		}
	}
	return (error);
}
