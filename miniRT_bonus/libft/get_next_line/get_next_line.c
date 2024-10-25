/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 18:49:13 by bwerner           #+#    #+#             */
/*   Updated: 2023/11/15 06:15:23 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void	shift_to_nl(char *buffer)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < BUFFER_SIZE && buffer[i] != '\n' && buffer[i] != '\0')
		i++;
	if (i >= BUFFER_SIZE || buffer[i] == '\0')
		return ;
	i++;
	while (i < BUFFER_SIZE && buffer[i] != '\0')
	{
		buffer[j] = buffer[i];
		i++;
		j++;
	}
	buffer[j] = '\0';
}

static char	*ft_strljoin(char const *s1, char const *s2, size_t len, t_gnl *s)
{
	s->i = 0;
	s->j = 0;
	while (s1[s->i] != '\0')
		s->i++;
	s->line = (char *)malloc(s->i + len + 1);
	s->i = 0;
	if (s->line == NULL)
		free((char *)s1);
	if (s->line == NULL)
		return (NULL);
	while (s1[s->i] != '\0')
	{
		s->line[s->i] = s1[s->i];
		s->i++;
	}
	while (s->j < len)
	{
		s->line[s->i] = s2[s->j];
		s->i++;
		s->j++;
	}
	s->line[s->i] = '\0';
	free((char *)s1);
	return (s->line);
}

static int	find_nl(char *str, t_gnl *s)
{
	int	i;

	i = 0;
	s->nl_found = 0;
	while (i < BUFFER_SIZE && str[i] != '\0')
	{
		if (str[i] == '\n')
			return (s->nl_found = 1, i);
		i++;
	}
	return (i - 1);
}

static char	*init_line(char *buffer, int fd, t_gnl *s)
{
	if (BUFFER_SIZE == 0)
		return (NULL);
	s->read = 0;
	s->nl_found = 0;
	if (buffer[0] == '\0')
	{
		s->read = read(fd, buffer, BUFFER_SIZE);
		if (s->read < 1)
			return (NULL);
		if (s->read < BUFFER_SIZE)
			buffer[s->read] = '\0';
	}
	s->line = (char *)malloc(1);
	if (s->line == NULL)
		return (NULL);
	s->line[0] = '\0';
	return (s->line);
}

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE];
	t_gnl		s;

	s.line = init_line(buffer, fd, &s);
	if (s.line == NULL)
		return (NULL);
	while (s.nl_found == 0 && buffer[0] != 0)
	{
		s.line = ft_strljoin(s.line, buffer, find_nl(buffer, &s) + 1, &s);
		if (s.line == NULL)
			return (NULL);
		if (s.nl_found == 0)
			s.read = read(fd, buffer, BUFFER_SIZE);
		if (s.read < 0 && s.nl_found == 0)
		{
			buffer[0] = '\0';
			free(s.line);
			return (NULL);
		}
		if (s.read < BUFFER_SIZE && s.nl_found == 0)
			buffer[s.read] = '\0';
	}
	shift_to_nl(buffer);
	return (s.line);
}
