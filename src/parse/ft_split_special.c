/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 09:43:57 by fefa              #+#    #+#             */
/*   Updated: 2025/05/18 18:22:50 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	get_word(char const *s, char *delimiters, char **a_word, size_t *j)
{
	int		i;
	char	*word;
	char	quote;

	i = 0;
	quote = 0;
	while (s[i] && (quote || !is_delimiter(s[i], delimiters)))
	{
		if (!quote && is_delimiter(s[i], "\'\""))
			quote = s[i];
		else if (quote == s[i])
			quote = 0;
		i++;
	}
	word = (char *)malloc(sizeof(char) * (i + 1));
	if (!word)
	{
		free_array(a_word);
		return ;
	}
	word[i] = '\0';
	while (--i >= 0)
		word[i] = s[i];
	*a_word = word;
	(*j) += ft_strlen(word);
}

static char	**split(char const *s, char *delimiters, char **a, size_t n)
{
	size_t	j;
	size_t	i;
	char	quote;

	i = 0;
	j = 0;
	while (i < n)
	{
		while (s[j] && is_delimiter(s[j], delimiters))
		{
			quote = 0;
			if (is_delimiter(s[j], "\'\""))
			{
				quote = s[j++];
				while (s[j] && s[j] != quote)
					j++;
			}
			j++;
		}
		get_word(&s[j], delimiters, &a[i], &j);
		if (!a[i++])
			return (free_array(a));
	}
	a[i] = NULL;
	return (a);
}

char	**ft_split_special(const char *s, char *c)
{
	char	**array;
	size_t	size;

	if (!s)
		return (0);
	size = count_words(s, c);
	array = (char **)malloc((size + 1) * sizeof(char *));
	if (!array)
		return (0);
	array = split(s, c, array, size);
	return (array);
}
