/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 19:34:52 by fefa              #+#    #+#             */
/*   Updated: 2025/05/08 21:26:40 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_export_single_word(char *arg, t_env *env, t_env *secret)
{
	t_env	*new;

	assign_env_node(&new, secret, arg, true);
	if (!new)
		return (1);
	add_env_end(&env, new);
	add_env_end(&secret, new);
	return (0);
}

// bool	ft_export(char *args[], t_env *env, t_env *secret)
// {
// 	size_t		i;
// 	bool		b;
// 	bool		tmp;

// 	i = 1;
// 	b = false;
// 	if (!args || !args[1])
// 		return (print_export_sort(secret));
// 	while (args[i])
// 	{
// 		tmp = ft_export_single_word(args[i++], env, secret);
// 		if (tmp == true)
// 			b = true;
// 	}
// 	return (b);
// }

// Modify the function signature
// bool ft_export(char *args[], t_env *env, t_env *secret)
// {
//     size_t      i;
//     bool        b;
//     bool        tmp;
    
//     i = 1;
//     b = false;
    
//     // Check if we're in a pipeline by examining if stdout is redirected
//     // This is a simple way to detect pipeline context
//     bool in_pipeline = !isatty(STDOUT_FILENO);
    
//     if (!args || !args[1])
//         return (print_export_sort(secret));
        
//     // In a pipeline, don't make permanent changes to the environment
//     if (in_pipeline)
//         return (0);
        
//     while (args[i])
//     {
//         tmp = ft_export_single_word(args[i++], env, secret);
//         if (tmp == true)
//             b = true;
//     }
//     return (b);
// }

bool is_valid_identifier(char *arg)
{
    int i;
    
    // Check if it's empty or just "="
    if (!arg || !*arg || *arg == '=')
        return (false);
    
    // First character must be alphabet or underscore
    if (!ft_isalpha(arg[0]) && arg[0] != '_')
        return (false);
    
    i = 0;
    // Find the position of '=' if any
    while (arg[i] && arg[i] != '=')
    {
        // All characters before '=' must be alphanumeric or underscore
        if (!ft_isalnum(arg[i]) && arg[i] != '_')
            return (false);
        i++;
    }
    
    return (true);
}

bool ft_export(char *args[], t_env *env, t_env *secret)
{
    size_t      i;
    bool        error;
    bool        tmp;
    bool        in_pipeline;
    
    i = 1;
    error = false;
    // Check if we're in a pipeline by examining if stdout is redirected
    in_pipeline = !isatty(STDOUT_FILENO);
    
    if (!args || !args[1])
        return (print_export_sort(secret));
    
    while (args[i])
    {
        // First, validate the identifier (regardless of pipeline context)
        if (!is_valid_identifier(args[i]))
        {
            ft_putstr_fd("minishell: export: `", STDERR_FILENO);
            ft_putstr_fd(args[i], STDERR_FILENO);
            ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
            error = true;
        }
        // Only attempt to export if we're not in a pipeline AND it's a valid identifier
        else if (!in_pipeline)
        {
            tmp = ft_export_single_word(args[i], env, secret);
            if (tmp == true)
                error = true;
        }
        i++;
    }
    
    return (error);
}
