/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader_program.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 14:39:42 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/27 16:00:40 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

void	create_shader_program(t_rt *rt)
{
	GLuint		shader_program;
	GLuint		shaders[2];
	char		*vert_src;
	char		*frag_src;

	shaders[0] = 0;
	shaders[1] = 0;

	// LOAD & ASSEMBLE SHADER SOURCES
	vert_src = assemble_shader_source("shaders/vertex/screen.vert");
	frag_src = assemble_shader_source("shaders/fragment/raytracer.frag");

	int fd = open("debug.frag", O_CREAT | O_RDWR, 0644);
	write(fd, frag_src, ft_strlen(frag_src));
	close(fd);

	// printf("%s\n", frag_src);

	if (vert_src == NULL || frag_src == NULL)
	{
		if (vert_src)
			free(vert_src);
		if (frag_src)
			free(frag_src);
		terminate("failed to assemble shader sources" ,1, rt);
	}

	// CREATE SHADERS FROM SOURCE_STRING

	shaders[0] = compile_shader_src(GL_VERTEX_SHADER, vert_src);
	shaders[1] = compile_shader_src(GL_FRAGMENT_SHADER, frag_src);
	free(vert_src);
	free(frag_src);
	if (shaders[0] == 0 || shaders[1] == 0)
	{
		if (shaders[0] > 0)
			glDeleteShader(shaders[0]);
		if (shaders[1] > 0)
			glDeleteShader(shaders[1]);
		terminate("failed to compile shaders", 1, rt);
	}

	// CREATE SHADER PROGRAM AND ATTACH SHADERS

	shader_program = glCreateProgram();
	glAttachShader(shader_program, shaders[0]);
	glAttachShader(shader_program, shaders[1]);

	// COMPILE SHADER PROGRAM

	int		success;
	// char	errorlog[512];
	glLinkProgram(shader_program);
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLint log_length;
		glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &log_length);
		char *errorlog = malloc(log_length);
		glGetProgramInfoLog(shader_program, log_length, NULL, errorlog);
		printf("ERROR SHADER PROGRAMM LINK FAILED\n%s\n", errorlog);
		free(errorlog);

		glDeleteShader(shaders[0]);
		glDeleteShader(shaders[1]);
		terminate("failed to create shader program", 1, rt);
	}

	// USE SHADER_PROGRAM && DELETE SHADER OBJECTS
	glUseProgram(shader_program);
	glDeleteShader(shaders[0]);
	glDeleteShader(shaders[1]);
	rt->shader_program = shader_program;
}
