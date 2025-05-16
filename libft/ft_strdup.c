/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albermud <albermud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 20:44:22 by fvargas           #+#    #+#             */
/*   Updated: 2025/05/16 20:35:47 by albermud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// char	*ft_strdup(const char *s)
// {
// 	char	*dest;
// 	size_t	i;

// 	dest = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
// 	if (!dest)
// 		return (0);
// 	i = 0;
// 	while (s[i])
// 	{
// 		dest[i] = s[i];
// 		i++;
// 	}
// 	dest[i] = '\0';
// 	return (dest);
// }

char *ft_strdup(const char *s)
{
    char *dest;
    size_t i;

    // Add NULL check
    if (!s)
    {
        dest = (char *)malloc(sizeof(char));
        if (!dest)
            return (0);
        dest[0] = '\0';
        return (dest);
    }

    dest = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
    if (!dest)
        return (0);
    i = 0;
    while (s[i])
    {
        dest[i] = s[i];
        i++;
    }
    dest[i] = '\0';
    return (dest);
}
