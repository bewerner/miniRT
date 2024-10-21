/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader_program.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 14:39:42 by nmihaile          #+#    #+#             */
/*   Updated: 2024/10/21 15:56:43 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static int	create_shader_sources(
	char **dst_vert, char **dst_frag, const char *vert, const char *frag)
{
	int	fd;
	int	x;

	*dst_vert = assemble_shader_source(vert);
	*dst_frag = assemble_shader_source(frag);
	if (*dst_vert == NULL || *dst_frag == NULL)
	{
		if (*dst_vert)
			ft_free((void *)dst_vert);
		if (*dst_frag)
			ft_free((void *)dst_frag);
		return (1);
	}
	fd = open("debug.frag", O_CREAT | O_RDWR | O_TRUNC, 0644);
	x = write(fd, *dst_frag, ft_strlen(*dst_frag));
	(void)x;
	close(fd);
	return (0);
}

static int	compile_shader_srcs(GLuint *shaders, char *vert_src, char *frag_src)
{
	shaders[0] = 0;
	shaders[1] = 0;
	shaders[0] = compile_shader_src(GL_VERTEX_SHADER, vert_src);
	shaders[1] = compile_shader_src(GL_FRAGMENT_SHADER, frag_src);
	ft_free((void *)&vert_src);
	ft_free((void *)&frag_src);
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
	char	*error_log;

	*shader_program = glCreateProgram();
	glAttachShader(*shader_program, shaders[0]);
	glAttachShader(*shader_program, shaders[1]);
	glLinkProgram(*shader_program);
	glDeleteShader(shaders[0]);
	glDeleteShader(shaders[1]);
	glGetProgramiv(*shader_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramiv(*shader_program, GL_INFO_LOG_LENGTH, &log_size);
		error_log = malloc(log_size);
		glGetProgramInfoLog(*shader_program, 512, NULL, error_log);
		printf("ERROR SHADER PROGRAMM LINK FAILED\n%s\n", error_log);
		ft_free((void *)&error_log);
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
	if (create_shader_sources(&vert_src, &frag_src, vert, frag))
		terminate("failed to assemble shader sources", NULL, 1, rt);
	if (compile_shader_srcs(shaders, vert_src, frag_src))
		terminate("failed to compile shaders", NULL, 1, rt);
	if (link_shader_program(shaders, &shader_program))
		terminate("failed to create shader program", NULL, 1, rt);
	glUseProgram(shader_program);
	return (shader_program);
}

void	init_shader_programs(t_rt *rt)
{
	rt->solid_shader_program = create_shader_program(
			"shaders/vertex/screen.vert",
			"shaders/solid/solid.frag", rt);
	rt->normal_shader_program = create_shader_program(
			"shaders/vertex/screen.vert",
			"shaders/normal/normal.frag", rt);
	rt->postprocessing_shader_program = create_shader_program(
			"shaders/vertex/screen.vert",
			"shaders/postprocessing/postprocessing.frag", rt);
	rt->preview_shader_program = create_shader_program(
			"shaders/vertex/screen.vert",
			"shaders/fragment/raytracer.frag", rt);
	rt->gizmo_shader_program = create_shader_program(
			"shaders/vertex/gizmo.vert",
			"shaders/gizmo/gizmo.frag", rt);
}
