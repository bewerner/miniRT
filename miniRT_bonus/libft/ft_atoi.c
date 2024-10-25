/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 18:03:51 by bwerner           #+#    #+#             */
/*   Updated: 2023/10/12 14:14:43 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	cspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r')
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	n;
	int	x;

	i = 0;
	n = 0;
	x = 1;
	while (cspace(str[i]) != 0)
		i++;
	if (str[i] == '-')
		x = -1;
	if (ft_isdigit(str[i]) != 0 || str[i] == '-' || str[i] == '+')
	{
		while (ft_isdigit(str[i + 1]) != 0)
			i++;
		while (i >= 0 && str[i] != '-' && str[i] != '+' && cspace(str[i]) == 0)
		{
			n += (str[i] - '0') * x;
			x *= 10;
			i--;
		}
	}
	return (n);
}
