/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 17:13:43 by bwerner           #+#    #+#             */
/*   Updated: 2024/03/14 15:12:02 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	locate_end(char const *s1, char const *set, int i, int j)
{
	i = ft_strlen(s1) - 1;
	if (set[0] == '\0')
		return (i);
	while (i >= 0)
	{
		if (s1[i] == set[j])
		{
			i--;
			j = 0;
		}
		else
			j++;
		if (set[j] == '\0')
			return (i);
	}
	return (i);
}

static int	locate_start(char const *s1, char const *set, int i, int j)
{
	if (set[0] == '\0')
		return (0);
	while (s1[i] != '\0')
	{
		if (s1[i] == set[j])
		{
			i++;
			j = 0;
		}
		else
			j++;
		if (set[j] == '\0')
			return (i);
	}
	return (i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start;
	int		end;
	char	*tstr;

	start = locate_start(s1, set, 0, 0);
	end = locate_end(s1, set, 0, 0);
	if (end == -1)
		return ((char *)ft_calloc(1, 1));
	tstr = (char *)ft_calloc(end - start + 2, 1);
	if (tstr == 0)
		return (0);
	ft_strlcpy(tstr, s1 + start, end - start + 2);
	return (tstr);
}
