/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 03:05:46 by bwerner           #+#    #+#             */
/*   Updated: 2023/10/15 05:20:07 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// calculates number length (including '-')
static int	n_len(int n)
{
	size_t	len;

	len = 1;
	if (n == -2147483648)
		return (11);
	if (n < 0)
	{
		len = 2;
		n = n * -1;
	}
	while (n >= 10)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char				*str;
	int					i;

	i = n_len(n) - 1;
	str = (char *)ft_calloc(i + 2, 1);
	if (str == NULL)
		return (NULL);
	if (n == -2147483648)
	{
		ft_memcpy(str, "-2147483648", 11);
		return (str);
	}
	if (n < 0)
	{
		str[0] = '-';
		n = n * -1;
	}
	while (i >= 0 && str[i] != '-')
	{
		str[i] = (n % 10 + '0');
		n = n / 10;
		i--;
	}
	return (str);
}
