/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset_int.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 19:55:06 by bwerner           #+#    #+#             */
/*   Updated: 2024/01/18 19:55:27 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset_int(void *b, int c, size_t len)
{
	size_t	i;
	int		*b_ptr;

	i = 0;
	b_ptr = b;
	while (i < len)
	{
		b_ptr[i] = c;
		i++;
	}
	return (b);
}
