/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader_assembler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 12:52:05 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/27 15:05:28 by nmihaile         ###   ########.fr       */
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

static size_t	get_import_file_size(char *str)
{
	size_t	size;

	size = 0;
	if (ft_strlen(str) > 8)
		size = file_size(prepares_import_filename(&str[8]));
	return (size);
}

static size_t	shader_src_size_with_import(const char *file)
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
			line_size = get_import_file_size(line);
		size += line_size;
		free(line);
		if (line_size == 0)
			return (close(fd), 0);
		line = get_next_line(fd);
	}
	close(fd);
	return (size);
}

static int	read_into_buf(char *buf, int fd)
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
			j = read_shader_src(&buf[i], prepares_import_filename(&line[8]));
			if (j == 0)
				return (-1);
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
