/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albbermu <albbermu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 21:51:40 by fefa              #+#    #+#             */
/*   Updated: 2025/05/09 14:56:39 by albbermu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_pwd(void)
{
	char	buffer[BUFFER];

	if (getcwd(buffer, BUFFER))
	{
		ft_putstr_fd(buffer, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		return (0);
	}
	return (1);
}
