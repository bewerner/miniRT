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

void	set_drawbuffers(t_rt *rt)
{
	static const GLenum	all[8] = {
		GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5,
		GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7};
	static const GLenum	one[8] = {
		GL_COLOR_ATTACHMENT0, GL_NONE, GL_NONE,
		GL_NONE, GL_NONE, GL_NONE, GL_NONE, GL_NONE};
	static const GLenum	diffuse_bounce[8] = {
		GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_NONE, GL_NONE, GL_NONE};
	static const GLenum	glossy_bounce[8] = {
		GL_COLOR_ATTACHMENT0, GL_NONE, GL_NONE,
		GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5,
		GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7};

	if (rt->mode != MODE_PREVIEW)
		glDrawBuffers(8, one);
	else if (rt->glossy_bounce_count > 0
		&& rt->glossy_bounce_count <= rt->max_glossy_bounces)
		glDrawBuffers(8, glossy_bounce);
	else if (rt->diffuse_bounce_count > 0
		&& rt->diffuse_bounce_count <= rt->max_diffuse_bounces)
		glDrawBuffers(8, diffuse_bounce);
	else
		glDrawBuffers(8, all);
}

void	bind_framebuffer(GLuint id, GLuint shader_program, t_rt *rt)
{
	GLint	uniform_location;

	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, rt->tex_fbo_id);
	if (shader_program == rt->preview_shader_program
		|| shader_program == rt->postprocessing_shader_program)
	{
		uniform_location = glGetUniformLocation(shader_program, "buffer");
		if (uniform_location == -1)
			terminate("buffer not found in shader program", NULL, 1, rt);
		glUniform1i(uniform_location, 0);
	}
	if (id == rt->fbo_id)
		set_drawbuffers(rt);
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
