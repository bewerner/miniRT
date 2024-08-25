/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader_assembler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 12:52:05 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/25 19:37:55 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static size_t	combined_src_size(const char **files)
{
	int		fd;
	size_t	total_size;
	off_t	curr_size;

	total_size = 0;
	while (*files)
	{
		fd = open(*files, O_RDONLY);
		if (fd == -1)
			return (0);
		curr_size = lseek(fd, 0, SEEK_END);
		if (curr_size == -1)
		{
			close(fd);
			return (0);
		}
		total_size += curr_size;
		close(fd);
		files++;
	}
	return (total_size);
}

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

char	*assemble_shader_source(const char **files)
{
	char	*shader_src;
	size_t	src_size;
	size_t	read_size;
	size_t	curr_pos;

	src_size = combined_src_size(files);
	if (src_size == 0)
		return (NULL);
	shader_src = (char *)malloc(src_size + 1 * sizeof(char));
	if (shader_src == NULL)
		return (NULL);
	curr_pos = 0;
	while (*files)
	{
		read_size = read_shader_src(&shader_src[curr_pos], *files);
		if (read_size == 0)
		{
			free(shader_src);
			return (NULL);
		}
		curr_pos += read_size;
		files++;
	}
	shader_src[curr_pos] = '\0';
	return (shader_src);
}
