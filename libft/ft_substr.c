/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 14:18:17 by bwerner           #+#    #+#             */
/*   Updated: 2023/10/15 02:20:05 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	int		i;
	size_t	s_len;
	char	*substr;

	if (s == NULL)
		return (NULL);
	i = 0;
	s_len = ft_strlen(s);
	if (s_len < start)
		len = 0;
	else if (s_len - start < len)
		len = s_len - start;
	substr = (char *)ft_calloc(len + 1, 1);
	if (substr == NULL)
		return (NULL);
	while (len - i > 0 && start + i < s_len)
	{
		substr[i] = s[start + i];
		i++;
	}
	return (substr);
}
