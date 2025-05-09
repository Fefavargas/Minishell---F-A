/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albbermu <albbermu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 14:04:07 by albbermu          #+#    #+#             */
/*   Updated: 2025/05/09 14:53:42 by albbermu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	trim_add_string(char **str, char ini_trim, char end_trim, char *add_str)
{
	char	*new_str;
	char	*s;
	int		j;
	int		i;
	int		k;

	s = *str;
	new_str = malloc(sizeof(char) * (ft_strlen(s) + ft_strlen(add_str) - end_trim + ini_trim));
	if (!new_str)
		return ;
	i = 0;
	k = 0;
	j = 0;
	if (!s[i])
	{
		add_string_middle(&new_str, add_str, 0);
		k = ft_strlen(add_str);
	}
	while (s[i])
	{
		if (i < ini_trim || i > end_trim)
			new_str[k++] = s[i];
		else
		{
			while (add_str && add_str[j])
				new_str[k++] = add_str[j++];
		}
		i++;
	}
	new_str[k] = 0;
	free(s);
	*str = new_str;
}

char	*substitui_str_with_env(char *str, int pos, t_mini *shell)
{
	int		i;
	char	*dup;
	char	*env_str;
	t_env	*env_var;

	i = pos;
	dup = ft_strdup(str);
	while (dup[i] && !is_delimiter(dup[i], "\'\" "))
		i++;
	if (dup[i])
		i--;
	env_str = ft_substr(dup, pos, i - pos + 1);
	env_var = get_env(shell->env, env_str);
	if (!env_var)
		trim_add_string(&dup, pos, i, "");
	else
		trim_add_string(&dup, pos, i, env_var->value);
	free(env_str);
	return (dup);
}

char	*change_dollar_sign(char *s, int pos, t_mini *shell)
{
	char	*new_str;
	char	*num_str;

	if (!s || s[pos] != '$' || !s[pos + 1] || is_delimiter(s[pos + 1], "\'\" "))
		return (s);
	trim_add_string(&s, pos, pos, "");
	if (s[pos] == '?')
	{
		trim_add_string(&s, pos, pos, "");
		num_str = ft_itoa(shell->exit_code);
		add_string_middle(&s, num_str, pos);
	}
	else if (s[pos])
	{
		new_str = substitui_str_with_env(s, pos, shell);
		return (new_str);
	}
	return (s);
}

void	expand_variable(char **str, t_mini *shell)
{
	int		i;
	char	*s;
	char	quote;
	char	*new_str;

	s = *str;
	i = 0;
	quote = 0;
	while (s[i])
	{
		if (quote != s[i] && s[i] == '\'')
			quote = '\'';
		else if (quote == s[i])
			quote = 0;
		if (!quote)
			s = change_dollar_sign(s, i, shell);
		if (s[i])
			i++;
	}
	new_str = remove_quotes(s);
	free(s);
	*str = new_str;
}
