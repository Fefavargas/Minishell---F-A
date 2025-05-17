/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 19:34:52 by fefa              #+#    #+#             */
/*   Updated: 2025/05/17 11:26:05 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_export_single_word(char *arg, t_env *env, t_env *secret)
{
	assign_env_node(secret, arg, true);
	assign_env_node(env, arg, true);
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

bool	ft_export(char *args[], t_env *env, t_env *secret)
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
		else if (ret == 2)
			ft_export_single_word(args[i], env, secret);
	}
	return (error);
}
