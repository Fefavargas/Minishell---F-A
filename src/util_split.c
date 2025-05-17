/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_split.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 09:43:57 by fefa              #+#    #+#             */
/*   Updated: 2025/05/17 19:56:30 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ignore_quotes_count(char const *s, size_t *i, \
							size_t *count, bool counter)
{
	char	quote;

	if (counter)
		(*count)++;
	if (!is_delimiter(s[*i], "\'\""))
		return ;
	quote = s[*i];
	(*i)++;
	while (s[*i] && s[*i] != quote)
		(*i)++;
	if (s[*i])
		(*i)++;
}

// static size_t	count_words(char const *s, char *delimiters)
// {
// 	size_t	count;
// 	size_t	i;

// 	i = 0;
// 	count = 0;
// 	while (s[i])
// 	{
// 		if (is_delimiter(s[i], "\'\""))
// 			ignore_quotes_count(s, &i, &count, TRUE);
// 		else if (!is_delimiter(s[i], delimiters))
// 		{
// 			count++;
// 			while (s[i] && !is_delimiter(s[i], delimiters))
// 			{
// 				if (is_delimiter(s[i], "\'\""))
// 					ignore_quotes_count(s, &i, &count, FALSE);
// 				if (i !=0 && is_delimiter(s[i], delimiters))
// 					i--;
// 				i++;
// 			}
// 		}
// 		else
// 			i++;
// 	}
// 	return (count);
// }

static size_t	count_words(char const *s, char *delimiters)
{
	size_t	count;
	size_t	i;

	i = 0;
	count = 0;
		while (s[i])
	{
		if (is_delimiter(s[i], "\'\""))
			ignore_quotes_count(s, &i, &count, TRUE);
		else if (!is_delimiter(s[i], delimiters))
		{
			count++;
			while (s[i] && !is_delimiter(s[i], delimiters))
			{
				// MODIFICATION START
				if (is_delimiter(s[i], "\'\""))
				{
					ignore_quotes_count(s, &i, &count, FALSE);
					// After ignore_quotes_count, i is updated.
					// s[i] could be '\0', a delimiter, or part of the word.
					// Continue to re-evaluate the inner loop's condition with the new i.
					continue;
				}
				// If not a quote, then s[i] is part of the current word
				// (guaranteed by inner loop condition). So, advance i.
				i++;
				// MODIFICATION END
			}
		}
		else
			i++;
	}
	return (count);
}

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
