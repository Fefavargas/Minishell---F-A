/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 14:04:07 by albbermu          #+#    #+#             */
/*   Updated: 2025/05/18 18:03:47 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*substitui_str_with_env(char *str, int pos, t_mini *shell)
{
	int		i;
	char	*dup;
	char	*env_name_str;
	t_env	*env_var;
	int		var_name_end_idx;

	i = pos;
	dup = ft_strdup(str);
	while (dup[i] && (ft_isalnum(dup[i]) || dup[i] == '_'))
		i++;
	if (i == pos)
		env_name_str = ft_substr(dup, pos, 0);
	else
		env_name_str = ft_substr(dup, pos, i - pos);
	env_var = get_env(shell->env, env_name_str);
	free(env_name_str);
	var_name_end_idx = i - 1;
	if (!env_var)
		trim_add_string(&dup, pos, var_name_end_idx, "");
	else
		trim_add_string(&dup, pos, var_name_end_idx, env_var->value);
	return (dup);
}

static char	*replace_dollar_value(char *s, size_t pos, t_mini *shell)
{
	char	*num_str;
	char	*result;
	char	*temp;

	if (s[pos] == '?')
	{
		trim_add_string(&s, pos, pos, "");
		num_str = ft_itoa(shell->exit_code);
		add_string_middle(&s, num_str, pos);
		free(num_str);
		return (s);
	}
	if (s[pos] == '$')
	{
		trim_add_string(&s, pos, pos, "");
		num_str = ft_itoa(getpid());
		add_string_middle(&s, num_str, pos);
		free(num_str);
		return (s);
	}
	temp = s;
	result = substitui_str_with_env(temp, pos, shell);
	free(temp);
	return (result);
}

char	*change_dollar_sign(char *s, size_t pos, t_mini *shell)
{
	if (!s || s[pos] != '$' || !s[pos + 1] || is_delimiter(s[pos + 1], "\'\" "))
		return (s);
	trim_add_string(&s, pos, pos, "");
	if (!s[pos])
		return (s);
	return (replace_dollar_value(s, pos, shell));
}

void	expand_variable(char **str, t_mini *shell)
{
	size_t	i;
	char	*s;
	char	quote;
	char	*new_str;

	s = *str;
	i = 0;
	quote = 0;
	while (s[i])
	{
		if ((s[i] == '\'' || s[i] == '\"') && quote == 0)
			quote = s[i];
		else if (s[i] == quote)
			quote = 0;
		if (quote != '\'' && s[i] == '$')
			s = change_dollar_sign(s, i, shell);
		if (s[i])
			i++;
	}
	new_str = remove_quotes(s);
	free(s);
	*str = new_str;
}
