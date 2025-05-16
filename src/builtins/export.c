/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 19:34:52 by fefa              #+#    #+#             */
/*   Updated: 2025/05/16 16:35:32 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// bool	ft_export_single_word(char *arg, t_env **env, t_env **secret)
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

	array = ft_split(str, '=');
	if (!array || !array[0] || !ft_isalpha(str[0]) || ft_strchr(array[0], '-'))
		return (0);
	valid = ft_strchr(str, '=');
	if (valid == 0)
		return (1);
	return (2);
}

// bool	is_valid_identifier(char *arg)
// {
// 	int	i;

// 	i = 0;
// 	if (!arg || !*arg || *arg == '=')
// 		return (false);
// 	if (!ft_isalpha(arg[0]) && arg[0] != '_')
// 		return (false);
// 	while (arg[i] && arg[i] != '=')
// 	{
// 		if (!ft_isalnum(arg[i]) && arg[i] != '_')
// 			return (false);
// 		i++;
// 	}
// 	return (true);
// }

// bool	ft_export(char *args[], t_env **env, t_env **secret)
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
