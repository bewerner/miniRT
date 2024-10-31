/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader_assembler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 12:52:05 by nmihaile          #+#    #+#             */
/*   Updated: 2024/10/31 07:18:59 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

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

static size_t	get_import_file_size(char *str, char *shader_path)
{
	size_t	size;
	char	import_file[1024];

	size = 0;
	if (ft_strlen(str) > 8)
	{
		prepares_import_filename(import_file, &str[8], shader_path);
		size = file_size(import_file);
	}
	return (size);
}

static size_t	shader_src_size_with_import(const char *file,
					size_t size, size_t line_size)
{
	int		fd;
	char	*line;
	char	shader_path[512];

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
			extract_shader_path(shader_path, file);
			line_size = get_import_file_size(line, shader_path);
		}
		size += line_size;
		ft_free((void *)&line);
		if (line_size == 0)
			return (close(fd), 0);
		line = get_next_line(fd);
	}
	close(fd);
	return (size);
}

static int	read_into_buf(char *buf, char *shader_path, int fd, size_t ij[2])
{
	char	*line;
	char	import_file[1024];

	ij[0] = 0;
	line = get_next_line(fd);
	while (line)
	{
		if (is_import(line))
		{
			prepares_import_filename(import_file, &line[8], shader_path);
			ij[1] = read_shader_src(&buf[ij[0]], import_file);
			if (ij[1] == 0)
				return (-1);
			ij[0] += ij[1];
		}
		else
		{
			ft_memcpy(&buf[ij[0]], line, ft_strlen(line));
			ij[0] += ft_strlen(line);
		}
		ft_free((void *)&line);
		line = get_next_line(fd);
	}
	buf[ij[0]] = '\0';
	return (0);
}

char	*assemble_shader_source(const char *file)
{
	int		fd;
	char	*shader_src;
	size_t	src_size;
	char	shader_path[512];

	src_size = shader_src_size_with_import(file, 0, 0);
	if (src_size == 0)
		return (NULL);
	shader_src = (char *)malloc(src_size + 1 * sizeof(char));
	fd = open(file, O_RDONLY);
	if (shader_src == NULL || fd == -1)
	{
		if (shader_src)
			ft_free((void *)&shader_src);
		return (NULL);
	}
	extract_shader_path(shader_path, file);
	if (read_into_buf(shader_src, shader_path, fd, (size_t [2]){0, 0}) == -1)
		ft_free((void *)&shader_src);
	close(fd);
	return (shader_src);
}
