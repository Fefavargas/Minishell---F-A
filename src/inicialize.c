/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inicialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 22:35:35 by fefa              #+#    #+#             */
/*   Updated: 2025/03/17 06:12:27 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_cmd(char *input, t_mini *shell)
{
	t_cmd	*cmd;
	char	**array;
	size_t	size;
	size_t	i;

	array = ft_split_special(input, "|;");
	size = 0;
	while (array[size])
		size++;
	cmd = (t_cmd *)malloc(sizeof(t_cmd) * (size + 1));
	if (!cmd)
		return; //ERROR
	i = 0;
	while (array[i])
	{
		cmd[i].cmd = array[i];
		if (i + 1 != size)
			cmd[i].next = &cmd[i + 1];
		cmd[i].words = ft_split_special(array[i], " ");
		create_tokens(&cmd[i]);
		i++;
	}
	cmd[size - 1].next = NULL;
	shell->cmd = cmd;
}

void	ft_join_free(char *s1, char *s2)
{
	char	*tmp;

	tmp = s1;
	s1 = ft_strjoin(tmp, s2);
	free(tmp);
}

void	set_value(t_env *node, char *env)
{
	char	**array;
	size_t	i;

	array = ft_split(env, '=');
	node->key = ft_strdup(array[0]);
	i = 0;
	while (array[++i])
	{
		ft_join_free(node->value, array[i]);
		if (array[i + 1])
			ft_join_free(node->value,"=");
	}
	free_array(array);
}

void	ft_copy_env(t_mini *shell, char **env)
{
	int		i;
	t_env	*node;

	i = -1;
	if (!(node = malloc(sizeof(t_env))))
		return ; // ERROR!
	shell->env = node;
	while (env[++i])
	{
		set_value(node, env[i]);
		if (env[i + 1])
		{
			if (!(node = malloc(sizeof(t_env))))
				return; // ERROR!
			node = node->next;
		}
	}
	node->next = NULL;
}

void	inic(t_mini *shell, char **env)
{
	shell->fdin = STDIN_FILENO;
	shell->fdout = STDOUT_FILENO;
	(void)env;
	//ft_copy_env(shell, env);
}
