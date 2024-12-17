/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 15:48:34 by bwerner           #+#    #+#             */
/*   Updated: 2023/10/14 16:55:05 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t		i;
	const char	*s_ptr;

	i = 0;
	s_ptr = s;
	while (i < n)
	{
		if (s_ptr[i] == (char)c)
			return ((void *)(s + i));
		i++;
	}
	return (0);
}
