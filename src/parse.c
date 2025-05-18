/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 12:25:23 by fefa              #+#    #+#             */
/*   Updated: 2025/05/18 18:36:37 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_open_quotes(char *line)
{
	size_t	i;
	bool	single_open;
	bool	double_open;

	i = 0;
	single_open = false;
	double_open = false;
	while (line[i])
	{
		if (!double_open && line[i] == '\'')
			single_open = !single_open;
		else if (!single_open && line[i] == '\"')
			double_open = !double_open;
		i++;
	}
	return (single_open || double_open);
}

static void	copy_with_insert(char *new_str, char *str, char *add, int pos)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	k = 0;
	if (!str[i])
	{
		while (add[j])
			new_str[k++] = add[j++];
	}
	while (str[i])
	{
		if (i == pos)
		{
			while (add[j])
				new_str[k++] = add[j++];
		}
		new_str[k++] = str[i++];
	}
	new_str[k] = '\0';
}

bool	add_string_middle(char **s, char *add, int pos)
{
	char	*new_str;
	char	*str;

	str = *s;
	new_str = malloc(sizeof(char) * (ft_strlen(str) + ft_strlen(add) + 1));
	if (!str || !add || !new_str)
		return (1);
	copy_with_insert(new_str, str, add, pos);
	free(str);
	*s = new_str;
	return (0);
}

bool	parse(char **input, t_mini *shell)
{
	if (is_blanked(*input))
		return (1);
	if (**input)
		add_history(*input);
	if (is_open_quotes(*input))
		return (error_msg("", "", "Error syntax with open quotes\n", 1));
	if (find_ampersand(*input))
		return (error_msg("", "", "Error syntax with &\n", 1));
	add_space_before(input, " ");
	add_space_after(input, " ");
	return (create_cmd_list(*input, shell));
}
