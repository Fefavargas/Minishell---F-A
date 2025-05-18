/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 14:04:07 by albbermu          #+#    #+#             */
/*   Updated: 2025/05/18 17:03:42 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	trim_add_string(char **str, char i_trim, char e_trim, char *add_str)
{
	char	*new_str;
	char	*s;
	int		j;
	int		i;
	int		k;
	int		new_len;
	int		add_str_len;

	i = 0;
	k = 0;
	j = 0;
	add_str_len = 0;
	s = *str;
	if (add_str)
		add_str_len = ft_strlen(add_str);
	new_len = ft_strlen(s) - (e_trim - i_trim + 1) + add_str_len + 1;
	if (new_len <= 0)
		new_len = 1;
	new_str = malloc(new_len);
	if (!new_str)
		return ;
	if (!s[i] && add_str)
	{
		while (add_str[j])
			new_str[k++] = add_str[j++];
	}
	while (s[i])
	{
		if (i < i_trim || i > e_trim)
			new_str[k++] = s[i];
		else if (i == i_trim)
		{
			while (add_str && add_str[j])
				new_str[k++] = add_str[j++];
		}
		i++;
	}
	new_str[k] = '\0';
	free(s);
	*str = new_str;
}

// void trim_add_string(char **str, char i_trim, char e_trim, char *add_str)
// {
//     char    *new_str;
//     char    *s;
//     int     j;
//     int     i;
//     int     k;
//     i = 0;
//     k = 0;
//     j = 0;
//     s = *str;
//     int new_len = ft_strlen(s) - (e_trim - i_trim + 1) + (add_str ? ft_strlen(add_str) : 0) + 1;
//     if (new_len <= 0)
//         new_len = 1;
//     new_str = malloc(new_len);
//     if (!new_str)
//         return;
//     if (!s[i] && add_str)
//     {
//         while (add_str[j])
//             new_str[k++] = add_str[j++];
//     }
//     while (s[i])
//     {
//         if (i < i_trim || i > e_trim)
//             new_str[k++] = s[i];
//         else if (i == i_trim)
//         {
//             while (add_str && add_str[j])
//                 new_str[k++] = add_str[j++];
//         }
//         i++;
//     }
//     new_str[k] = '\0';
//     free(s);
//     *str = new_str;
// }

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

char	*change_dollar_sign(char *s, int pos, t_mini *shell)
{
	char	*num_str;
	char	*result_str;
	char	*temp_s_before_subst;

	if (!s || s[pos] != '$' || !s[pos + 1] || is_delimiter(s[pos + 1], "\'\" "))
		return (s);
	trim_add_string(&s, pos, pos, "");
	if (s[pos] == '?')
	{
		trim_add_string(&s, pos, pos, "");
		num_str = ft_itoa(shell->exit_code);
		add_string_middle(&s, num_str, pos);
		free(num_str);
		result_str = s;
	}
	else if (s[pos] == '$')
	{
		trim_add_string(&s, pos, pos, "");
		num_str = ft_itoa(getpid());
		add_string_middle(&s, num_str, pos);
		free(num_str);
		result_str = s;
	}
	else if (s[pos])
	{
		temp_s_before_subst = s;
		result_str = substitui_str_with_env(temp_s_before_subst, pos, shell);
		free(temp_s_before_subst);
	}
	else
		result_str = s;
	return (result_str);
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

// char *change_dollar_sign(char *s, int pos, t_mini *shell)
// {
//     char *new_str;
//     char *num_str;

//     if (!s || s[pos] != '$' || !s[pos + 1] || is_delimiter(s[pos + 1], "\'\" "))
//         return (ft_strdup(s));
//     new_str = ft_strdup(s);
//     if (!new_str)
//         return (ft_strdup(s));
//     trim_add_string(&new_str, pos, pos, "");
//     if (new_str[pos] == '?')
//     {
//         trim_add_string(&new_str, pos, pos, "");
//         num_str = ft_itoa(shell->exit_code);
//         if (num_str)
//         {
//             add_string_middle(&new_str, num_str, pos);
//             free(num_str);
//         }
//     }
//     else if (new_str[pos] == '$')
//     {
//         trim_add_string(&new_str, pos, pos, "");
//         num_str = ft_itoa(getpid());
//         if (num_str)
//         {
//             add_string_middle(&new_str, num_str, pos);
//             free(num_str);
//         }
//     }
//     else
//     {
//         char *tmp = substitui_str_with_env(new_str, pos, shell);
//         free(new_str);
//         return tmp;
//     }
//     return new_str;
// }

// void expand_variable(char **str, t_mini *shell)
// {
//     int     i;
//     char    *s;
//     char    quote;
//     char    *new_str;
//     char    *tmp;

//     if (!str || !*str || !**str)
//         return;
//     s = *str;
//     i = 0;
//     quote = 0;
//     while (s[i])
//     {
//         if ((s[i] == '\'' || s[i] == '\"') && quote == 0)
//             quote = s[i];
//         else if (s[i] == quote)
//             quote = 0;
//         if (quote != '\'' && s[i] == '$')
//         {
//             tmp = change_dollar_sign(s, i, shell);
//             free(s);
//             s = tmp;
//             i = 0;
//             continue;
//         }
//         if (s[i])
//             i++;
//     }
//     new_str = remove_quotes(s);
//     free(s);
//     *str = new_str;
// }
