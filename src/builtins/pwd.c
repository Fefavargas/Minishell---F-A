/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 21:51:40 by fefa              #+#    #+#             */
/*   Updated: 2025/04/05 18:41:45 by albermud         ###   ########.fr       */
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
		return (SUCCESS);
	}
	return (ERROR);
}
