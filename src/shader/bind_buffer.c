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
	// (void)shader_program;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rt->tex_fbo_id);
	glUniform1i(glGetUniformLocation(shader_program, "cumulative_render_buffer"), 0); // this line is not needed in wsl. check for mac and linux
}

// void	bind_framebuffer_textures(GLuint shader_program, t_rt *rt)
// {
// 	// (void)shader_program;
// 	glActiveTexture(GL_TEXTURE0);
// 	glBindTexture(GL_TEXTURE_2D, rt->tex_fbo_id);
// 	glUniform1i(glGetUniformLocation(shader_program, "cumulative_render_buffer"), 0); // this line is not needed in wsl. check for mac and linux
// 	glActiveTexture(GL_TEXTURE0 + 12);
// 	glBindTexture(GL_TEXTURE_2D, rt->tex_fbo_id2);
// 	glUniform1i(glGetUniformLocation(shader_program, "g_hitpoint_pos"), 12); // this line is not needed in wsl. check for mac and linux
// 	glActiveTexture(GL_TEXTURE0 + 13);
// 	glBindTexture(GL_TEXTURE_2D, rt->tex_fbo_id3);
// 	glUniform1i(glGetUniformLocation(shader_program, "g_hitpoint_normal"), 13); // this line is not needed in wsl. check for mac and linux
// 	glActiveTexture(GL_TEXTURE0 + 14);
// 	glBindTexture(GL_TEXTURE_2D, rt->tex_fbo_id4);
// 	glUniform1i(glGetUniformLocation(shader_program, "g_hitpoint_render"), 14); // this line is not needed in wsl. check for mac and linux
// }

void	bind_framebuffer_textures(GLuint shader_program, t_rt *rt)
{
	// (void)shader_program;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, rt->tex_fbo_id);
	glUniform1i(glGetUniformLocation(shader_program, "buffer"), 0); // this line is not needed in wsl. check for mac and linux
}

void	bind_objects(GLuint shader_program, t_rt *rt)
{
	GLint uniform_location;

	// glBindBuffer(GL_TEXTURE_BUFFER, rt->tbo_objects_id);
	// glActiveTexture(GL_TEXTURE1);
	// glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, rt->tbo_objects_id);
	uniform_location = glGetUniformLocation(shader_program, "objects");
	if (uniform_location == -1)
		terminate("objects not found in shader program", NULL, 1, rt);
	glUniform1i(uniform_location, 1);
	// glBindBuffer(GL_TEXTURE_BUFFER, 0);	// unbind
}

void	bind_lights(GLuint shader_program, t_rt *rt)
{
	GLint uniform_location;

	// glBindBuffer(GL_TEXTURE_BUFFER, rt->tbo_lights_id);
	// glActiveTexture(GL_TEXTURE2);
	// glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, rt->tbo_lights_id);
	uniform_location = glGetUniformLocation(shader_program, "lights");
	if (uniform_location == -1)
		terminate("lights not found in shader program", NULL, 1, rt);
	glUniform1i(uniform_location, 2);
	// glBindBuffer(GL_TEXTURE_BUFFER, 0);	// unbind
}

void	bind_agx_lut(GLuint shader_program, t_rt *rt)
{
	GLint uniform_location;

	// glBindBuffer(GL_TEXTURE_BUFFER, rt->tbo_agx_lut_id);
	// glActiveTexture(GL_TEXTURE3);
	// glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, rt->tbo_agx_lut_id);
	uniform_location = glGetUniformLocation(shader_program, "agx_lut");
	if (uniform_location == -1)
		terminate("agx_lut not found in shader program", NULL, 1, rt);
	glUniform1i(uniform_location, 3);
	// glBindBuffer(GL_TEXTURE_BUFFER, 0);	// unbind
}

void	bind_environment_map(GLuint shader_program, t_rt *rt)
{
	GLint uniform_location;

	uniform_location = glGetUniformLocation(shader_program, "environment_map");
	if (uniform_location == -1)
		terminate("environment_map not found in shader program", NULL, 1, rt);
	glUniform1i(uniform_location, 4);
}

void	bind_texture_units(GLuint shader_program, t_rt *rt)
{
	size_t		idx;
	GLint 		uniform_location;
	static char	*unit_names[3] = {
		"texture_units[0]",
		"texture_units[1]",
		"texture_units[2]"};

	idx = 0;
	while (idx < MAX_IMAGE_TEXTURES)
	// while (idx < 1)
	{
		uniform_location = glGetUniformLocation(shader_program, unit_names[idx]);
		// uniform_location = glGetUniformLocation(shader_program, "texture_units[0]");
		if (uniform_location == -1)
			terminate("texture_units not found in shader program", NULL, 1, rt);
		// glActiveTexture(GL_TEXTURE5 + idx);
		// glBindTexture(GL_TEXTURE_2D, rt->texture_ids[idx]);
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
