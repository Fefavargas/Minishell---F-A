/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 18:50:32 by albermud          #+#    #+#             */
/*   Updated: 2025/05/18 19:05:28 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*substr(const char *s, int start, int len)
{
	char	*res;
	int		i;

	res = malloc(len + 1);
	if (!res)
		return (NULL);
	i = 0;
	while (i < len && s[start + i])
	{
		res[i] = s[start + i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

static char	*strjoin_three(const char *s1, const char *s2, const char *s3)
{
	char	*res;
	size_t	len;
	size_t	i;
	size_t	j;

	if (!s1 || !s2 || !s3)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3);
	res = malloc(len + 1);
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[j])
		res[i++] = s1[j++];
	j = 0;
	while (s2[j])
		res[i++] = s2[j++];
	j = 0;
	while (s3[j])
		res[i++] = s3[j++];
	res[i] = '\0';
	return (res);
}

static const char	*get_add_str(const char *s)
{
	if (s)
		return (s);
	return ("");
}

void	trim_add_string(char **str, int i_trim, int e_trim, const char *add_str)
{
	char	*old;
	char	*prefix;
	char	*suffix;
	char	*new_str;

	old = *str;
	prefix = substr(old, 0, i_trim);
	if (!prefix)
		return ;
	suffix = substr(old, e_trim + 1,
			ft_strlen(old) - (e_trim + 1));
	if (!suffix)
	{
		free(prefix);
		return ;
	}
	new_str = strjoin_three(prefix, get_add_str(add_str), suffix);
	free(prefix);
	free(suffix);
	if (!new_str)
		return ;
	free(old);
	*str = new_str;
}
