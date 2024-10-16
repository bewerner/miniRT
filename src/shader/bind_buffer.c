/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bind_buffer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 01:39:19 by bwerner           #+#    #+#             */
/*   Updated: 2024/09/22 01:07:35 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

void	bind_framebuffer_texture(GLuint shader_program, t_rt *rt)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rt->tex_fbo_id);
	glUniform1i(glGetUniformLocation(shader_program, "cumulative_render_buffer"), 0); // this line is not needed in wsl. check for mac and linux
}

void	bind_objects(GLuint shader_program, t_rt *rt)
{
	GLint	uniform_location;

	uniform_location = glGetUniformLocation(shader_program, "objects");
	if (uniform_location == -1)
		terminate("objects not found in shader program", NULL, 1, rt);
	glUniform1i(uniform_location, 1);
}

void	bind_lights(GLuint shader_program, t_rt *rt)
{
	GLint	uniform_location;

	uniform_location = glGetUniformLocation(shader_program, "lights");
	if (uniform_location == -1)
		terminate("lights not found in shader program", NULL, 1, rt);
	glUniform1i(uniform_location, 2);
}

void	bind_agx_lut(GLuint shader_program, t_rt *rt)
{
	GLint	uniform_location;

	uniform_location = glGetUniformLocation(shader_program, "agx_lut");
	if (uniform_location == -1)
		terminate("agx_lut not found in shader program", NULL, 1, rt);
	glUniform1i(uniform_location, 3);
}

void	bind_environment_map(GLuint shader_program, t_rt *rt)
{
	GLint	uniform_location;

	uniform_location = glGetUniformLocation(shader_program, "environment_map");
	if (uniform_location == -1)
		terminate("environment_map not found in shader program", NULL, 1, rt);
	glUniform1i(uniform_location, 4);
}

void	bind_texture_units(GLuint shader_program, t_rt *rt)
{
	size_t		idx;
	GLint		uniform_location;
	static char	*unit_names[10] = {
		"texture_units[0]",
		"texture_units[1]",
		"texture_units[2]",
		"texture_units[3]",
		"texture_units[4]",
		"texture_units[5]",
		"texture_units[6]",
		"texture_units[7]",
		"texture_units[8]",
		"texture_units[9]"};

	idx = 0;
	while (idx < MAX_IMAGE_TEXTURES)
	{
		uniform_location
			= glGetUniformLocation(shader_program, unit_names[idx]);
		if (uniform_location == -1)
			terminate("texture_units not found in shader program", NULL, 1, rt);
		glUniform1i(uniform_location, 5 + idx);
		idx++;
	}
}

void	bind_material_ubo(GLuint shader_program, t_rt *rt)
{
	GLuint	block_index;

	block_index = glGetUniformBlockIndex(shader_program, "u_materials");
	if (block_index == GL_INVALID_INDEX)
		terminate("materials ubo not found in shader program", NULL, 1, rt);
	glUniformBlockBinding(shader_program, block_index, 1);
}

void	bind_texture_ubo(GLuint shader_program, t_rt *rt)
{
	GLuint	block_index;

	block_index = glGetUniformBlockIndex(shader_program, "u_textures");
	if (block_index == GL_INVALID_INDEX)
		terminate("textures ubo not found in shader program", NULL, 1, rt);
	glUniformBlockBinding(shader_program, block_index, 2);
}
