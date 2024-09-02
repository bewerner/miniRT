/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 17:06:51 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/31 17:22:46 by nmihaile         ###   ########.fr       */
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

void	ft_terminate_after_word(char *str)
{
	if (str == NULL)
		return ;
	while (*str && !ft_isspace(*str))
		str++;
	*str = '\0';
}
