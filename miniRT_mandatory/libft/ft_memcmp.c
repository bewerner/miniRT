/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 16:32:51 by bwerner           #+#    #+#             */
/*   Updated: 2023/10/12 14:15:18 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t		i;
	const char	*s1_ptr;
	const char	*s2_ptr;

	i = 0;
	s1_ptr = s1;
	s2_ptr = s2;
	if (n == 0)
		return (0);
	while (s1_ptr[i] == s2_ptr[i] && i < n - 1)
		i++;
	return ((unsigned char)s1_ptr[i] - (unsigned char)s2_ptr[i]);
}
