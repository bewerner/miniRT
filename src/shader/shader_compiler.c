/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader_compiler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 10:31:58 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/27 18:25:13 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

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

void	extract_shader_path(char *shader_path, const char *file)
{
	char	*end;
	int		size;

	ft_memset(shader_path, 0, 512);
	end = ft_strrchr(file, '/');
	if (end == 0)
		return ;
	size = (++end) - file;
	if (size > 0 && size < 512)
		ft_strlcat(shader_path, file, end - file + 1);
}

void	prepares_import_filename(char *filename, char *str, char *shader_path)
{
	ft_memset(filename, 0, 1024);
	ft_strlcat(filename, shader_path, 1024);
	if (str[ft_strlen(str) - 1] == '\n')
		str[ft_strlen(str) - 1] = '\0';
	ft_strlcat(filename, str, 1024);
}

GLuint	compile_shader_src(GLenum shader_type, const char *shader_src)
{
	GLuint	shader_id;
	int		success;
	char	errorlog[512];

	if (shader_src == NULL)
		return (-1);
	shader_id = glCreateShader(shader_type);
	glShaderSource(shader_id, 1, &shader_src, NULL);
	glCompileShader(shader_id);
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE)
	{
		glGetShaderInfoLog(shader_id, 512, NULL, errorlog);
		printf("ERROR SHADER COMPILATION FAILED\n%s\n", errorlog);
		return (0);
	}
	return (shader_id);
}
