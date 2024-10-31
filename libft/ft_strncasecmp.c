/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncasecmp.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 19:54:55 by bwerner           #+#    #+#             */
/*   Updated: 2024/06/05 19:55:19 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncasecmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (ft_tolower(s1[i]) == ft_tolower(s2[i]) && i < n)
	{
		if (s1[i] == '\0' || i == n - 1)
			return (0);
		i++;
	}
	return ((unsigned char)ft_tolower(s1[i])
		- (unsigned char)ft_tolower(s2[i]));
}
