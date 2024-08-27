/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader_assembler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 12:52:05 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/27 14:50:25 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static size_t	read_shader_src(char *buf, const char *file)
{
	int		fd;
	size_t	size;

	fd = open(file, O_RDONLY);
	if (fd == -1)
		return (0);
	size = 0;
	while (read(fd, buf++, 1) > 0)
		size++;
	close(fd);
	return (size);
}

size_t	file_size(const char *file)
{
	int		fd;
	off_t	size;

	fd = open(file, O_RDONLY);
	if (fd == -1)
		return (0);
	size = lseek(fd, 0, SEEK_END);
	if (size == -1)
		size = 0;
	close(fd);
	return ((size_t)size);
}

int	is_import(char *str)
{
	return (ft_strncmp(str, "#import ", 8) == 0);
}

size_t	shader_src_size_with_import(const char *file)
{
	int		fd;
	size_t	size;
	size_t	line_size;
	char	*line;

	size = 0;
	fd = open(file, O_RDONLY);
	if (fd == -1)
		return (0);
	line = get_next_line(fd);
	while (line)
	{
		line_size = ft_strlen(line);
		if (is_import(line))
		{
			if (ft_strlen(line) > 8)
			{
				if (line[ft_strlen(line) - 1] == '\n')
					line[ft_strlen(line) - 1] = '\0';
				line_size = file_size(&line[8]);
				printf("%s -> %lu\n", &line[8], line_size);
			}
			else
				line_size = 0;
		}
		size += line_size;
		free(line);
		if (line_size == 0)
			return (0);
		line = get_next_line(fd);
	}
	close(fd);
	return (size);
}

int	read_into_buf(char *buf, int fd)
{
	size_t	i;
	size_t	j;
	char	*line;

	i = 0;
	line = get_next_line(fd);
	while (line)
	{
		if (is_import(line))
		{
			if (line[ft_strlen(line) - 1] == '\n')
				line[ft_strlen(line) - 1] = '\0';
			j = read_shader_src(&buf[i], &line[8]);
			if (j == 0)
			{
				return (close(fd), -1);
			}
			i += j;
		}
		else
		{
			ft_memcpy(&buf[i], line, ft_strlen(line));
			i += ft_strlen(line);
		}
		free(line);
		line = get_next_line(fd);
	}
	buf[i] = '\0';
	return (0);
}

char	*assemble_shader_source(const char *file)
{
	int		fd;
	char	*shader_src;
	size_t	src_size;

	src_size = shader_src_size_with_import(file);
	if (src_size == 0)
		return (NULL);
	shader_src = (char *)malloc(src_size + 1 * sizeof(char));
	fd = open(file, O_RDONLY);
	if (shader_src == NULL || fd == -1)
	{
		if (shader_src)
			free(shader_src);
		return (NULL);
	}
	if (read_into_buf(shader_src, fd) == -1)
	{
		free (shader_src);
		shader_src = NULL;
	}
	close(fd);
	return (shader_src);
}
