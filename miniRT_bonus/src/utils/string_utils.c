/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 17:06:51 by nmihaile          #+#    #+#             */
/*   Updated: 2024/10/23 21:03:28 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

void	whitespace_to_space(char *str)
{
	int		i;

	if (str == NULL)
		return ;
	i = 0;
	while (str[i] != '\0')
	{
		if (ft_isspace(str[i]))
			str[i] = ' ';
		i++;
	}
}

void	ft_skipspace(char **str)
{
	while (*str && ft_isspace(**str))
		(*str)++;
}

int	ft_strcmp(const char *s1, const char *s2)
{
	if (s1 == NULL || s2 == NULL)
	{
		if (s1 == s2)
			return (0);
		if (s1 == NULL)
			return (-1);
		else
			return (1);
	}
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

char	*ft_terminate_after_word(char *str)
{
	char	*pointer;

	pointer = NULL;
	if (str == NULL)
		return (NULL);
	while (*str && !ft_isspace(*str))
		str++;
	if (*str != '\0')
		pointer = str + 1;
	*str = '\0';
	return (pointer);
}
