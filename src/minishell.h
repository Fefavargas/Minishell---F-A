/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 09:09:23 by fefa              #+#    #+#             */
/*   Updated: 2025/03/16 08:13:29 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stddef.h>
# include <stdio.h> // perror()
# include <string.h>
# include <stdlib.h> // exit() - free()
# include <unistd.h> // write() - close()
# include <readline/readline.h> // readline()
# include <readline/history.h> // add_history()
# include <stdbool.h>
# include <fcntl.h> //open() - close()
# include <sys/types.h>
# include <sys/wait.h> // waitpid()
# include <errno.h>
# include "../libft/libft.h"

typedef	struct	s_mini
{
	int		fdin;
	int		fdout;
	
}	t_mini;

#endif