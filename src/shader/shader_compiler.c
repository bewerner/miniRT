/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader_compiler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 10:31:58 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/27 15:00:41 by nmihaile         ###   ########.fr       */
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

char	*prepares_import_filename(char *str)
{
	if (str[ft_strlen(str) - 1] == '\n')
		str[ft_strlen(str) - 1] = '\0';
	return (str);
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
