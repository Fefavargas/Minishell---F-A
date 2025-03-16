/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fefa <fefa@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 22:32:44 by fvargas           #+#    #+#             */
/*   Updated: 2025/03/16 12:54:07 by fefa             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
	Malloc a new string starting from START until the size LEN or end of the s.
 */
char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	size;

	if (!s)
		return (0);
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	size = ft_strlen(&s[start]);
	if (len > size)
		len = size;
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (0);
	ft_strlcpy(str, &s[start], len + 1);
	return (str);
}
