/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 14:34:08 by bwerner           #+#    #+#             */
/*   Updated: 2023/10/12 14:15:59 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	nlen;

	i = 0;
	nlen = ft_strlen(needle);
	if (needle[0] == '\0')
		return ((char *)haystack);
	while (haystack[i] != '\0' && nlen + i <= len)
	{
		if (ft_strncmp(haystack + i, needle, nlen) == 0)
			return ((char *)haystack + i);
		i++;
	}
	return (0);
}
