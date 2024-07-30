/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 16:47:26 by bwerner           #+#    #+#             */
/*   Updated: 2023/10/12 14:15:31 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t		i;
	char		*dst_ptr;
	const char	*src_ptr;

	i = 0;
	dst_ptr = dst;
	src_ptr = src;
	if (dst_ptr == 0 && src_ptr == 0)
		return (0);
	while (i < len)
	{
		if (dst_ptr < src_ptr)
		{
			dst_ptr[i] = src_ptr[i];
			i++;
		}
		else
		{
			dst_ptr[len - 1 - i] = src_ptr[len - 1 - i];
			i++;
		}
	}
	return (dst);
}
