/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 09:43:57 by fefa              #+#    #+#             */
/*   Updated: 2025/05/05 21:46:20 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_delimiter(char c, const char *delimiters)
{
	size_t	i;

	i = 0;
	while (delimiters[i])
	{
		if (c == delimiters[i++])
			return (TRUE);
	}
	return (FALSE);
}

void	ignore_quotes(char const *s, int *i)
{
	char	quote;

	if (!is_delimiter(s[*i], "\'\""))
		return ;
	quote = s[*i];
	(*i)++;
	while (s[*i] && s[*i] != quote)
		(*i)++;
	if (s[*i])
		(*i)++;
}

static size_t	count_words(char const *s, char *delimiters)
{
	size_t	count;
	int		i;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (is_delimiter(s[i], "\'\""))
		{
			count++;
			ignore_quotes(s, &i);
		}
		else if (!is_delimiter(s[i], delimiters))
		{
			count++;
 			while (s[i] && !is_delimiter(s[i], delimiters))
			{
				if (is_delimiter(s[i], "\'\""))
					ignore_quotes(s, &i);
				i++;
			}
		}
		else
			i++;
	}
	return (count);
}

static void	free_memory(size_t item_count, char **dest)
{
	while (item_count > 0)
	{
		item_count--;
		free(*(dest + item_count));
	}
	free(dest);
}

static char	*get_word(char const *s, char *delimiters)
{
	size_t	i;
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
		return (0);
	word[i] = '\0';
	i--;
	while (i > 0)
	{
		word[i] = s[i];
		i--;
	}
	word[i] = s[i];
	return (word);
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
		a[i] = get_word(&s[j], delimiters);
		if (!a[i])
		{
			free_memory(i, a);
			return (0);
		}
		j += ft_strlen(a[i]);
		i++;
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
