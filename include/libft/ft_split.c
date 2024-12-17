/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 15:19:20 by bwerner           #+#    #+#             */
/*   Updated: 2023/11/14 23:20:27 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// count number of substrings
static int	countstrings(char const *s, char c, int i, int str_count)
{
	while (s[i] != '\0')
	{
		if (s[i] == c)
			i++;
		else
		{
			str_count++;
			while (s[i] != c && s[i] != '\0')
				i++;
		}
	}
	return (str_count);
}

// free everything if allocating arr[i] failed
static int	mem_isgood(char **arr, int i)
{
	if (arr[i] == NULL)
	{
		i--;
		while (i >= 0)
		{
			free(arr[i]);
			i--;
		}
		free(arr);
		return (0);
	}
	return (1);
}

// find index of first character of substring number n
static int	find_start(int n, char const *s, char c, int start)
{
	int	i;

	i = 0;
	while (i <= n)
	{
		while (s[start] != c && s[start] != '\0' && i != 0)
			start++;
		while (s[start] == c && s[start] != '\0')
			start++;
		i++;
	}
	return (start);
}

// find length of substring number n
static int	find_len(int n, char const *s, char c, int end)
{
	int	i;

	i = 0;
	while (i <= n)
	{
		while (s[end] == c && s[end] != '\0')
			end++;
		while (s[end] != c && s[end] != '\0')
			end++;
		i++;
	}
	return (end - find_start(n, s, c, 0));
}

char	**ft_split(char const *s, char c)
{
	int		i;
	int		str_count;
	char	**arr;

	if (s == NULL)
		return (NULL);
	i = 0;
	str_count = countstrings(s, c, 0, 0);
	arr = (char **)ft_calloc(str_count + 1, sizeof(char *));
	if (arr == NULL)
		return (NULL);
	while (i < str_count)
	{
		arr[i] = ft_substr(s, find_start(i, s, c, 0), find_len(i, s, c, 0));
		if (mem_isgood(arr, i) == 0)
			return (NULL);
		i++;
	}
	return (arr);
}
