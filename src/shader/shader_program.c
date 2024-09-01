/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader_program.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 14:39:42 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/31 11:03:55 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static int	create_shader_sources(char **dst_vert, char **dst_frag, const char *vert, const char *frag)
{
	*dst_vert = assemble_shader_source(vert);
	*dst_frag = assemble_shader_source(frag);

	// SAVE DEBUG FRAG-SHADER-SRC
	int fd = open("debug.frag", O_CREAT | O_RDWR | O_TRUNC, 0644);
	int x = write(fd, *dst_frag, ft_strlen(*dst_frag));
	(void)x;
	close(fd);

	if (*dst_vert == NULL || *dst_frag == NULL)
	{
		if (*dst_vert)
			free(*dst_vert);
		if (*dst_frag)
			free(*dst_frag);
		return (1);
	}
	return (0);
}

static int	compile_shader_srcs(GLuint *shaders, char *vert_src, char *frag_src)
{
	shaders[0] = 0;
	shaders[1] = 0;
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
		return (1);
	}
	return (0);
}

static int	link_shader_program(GLuint *shaders, GLuint *shader_program)
{
	int		success;
	GLint	log_size;
	char 	*errorlog;

	// ATTACH SHADERS
	*shader_program = glCreateProgram();
	glAttachShader(*shader_program, shaders[0]);
	glAttachShader(*shader_program, shaders[1]);

	// LINK SHADERS AND PROGRAM
	glLinkProgram(*shader_program);
	glDeleteShader(shaders[0]);
	glDeleteShader(shaders[1]);
	glGetProgramiv(*shader_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramiv(*shader_program, GL_INFO_LOG_LENGTH, &log_size);
		errorlog = malloc(log_size);
		glGetProgramInfoLog(*shader_program, 512, NULL, errorlog);
		printf("ERROR SHADER PROGRAMM LINK FAILED\n%s\n", errorlog);
		free(errorlog);
		return (1);
	}
	return (0);
}

GLuint	create_shader_program(const char *vert, const char *frag, t_rt *rt)
{
	GLuint		shader_program;
	GLuint		shaders[2];
	char		*vert_src;
	char		*frag_src;

	shader_program = 0;

	// LOAD & ASSEMBLE SHADER SOURCES
	if (create_shader_sources(&vert_src, &frag_src, vert, frag))
		terminate("failed to assemble shader sources" , 1, rt);

	// CREATE SHADERS FROM SOURCE_STRING
	if (compile_shader_srcs(shaders, vert_src, frag_src))
		terminate("failed to compile shaders", 1, rt);

	// CREATE SHADER PROGRAM
	if (link_shader_program(shaders, &shader_program))
		terminate("failed to create shader program", 1, rt);

	// USE SHADER_PROGRAM
	glUseProgram(shader_program);
	return (shader_program);
}
