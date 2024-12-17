/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_ws.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 15:19:20 by bwerner           #+#    #+#             */
/*   Updated: 2023/12/14 05:44:52 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	whitespace_to_space(char *str)
{
	int		i;

	i = 0;
	while (str[i] != '\0')
	{
		if (ft_isspace(str[i]))
			str[i] = ' ';
		i++;
	}
}

char	**ft_split_ws(char *str)
{
	char	**arr;

	whitespace_to_space(str);
	arr = ft_split(str, ' ');
	return (arr);
}
