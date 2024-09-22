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
	glUniform1i(glGetUniformLocation(shader_program, "raw_render_image"), 0); // this line is not needed in wsl. check for mac and linux
}

void	bind_objects(GLuint shader_program, t_rt *rt)
{
	GLint uniform_location;

	// glBindBuffer(GL_TEXTURE_BUFFER, rt->tbo_objects_id);
	// glActiveTexture(GL_TEXTURE1);
	// glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, rt->tbo_objects_id);
	(void)rt;
	uniform_location = glGetUniformLocation(shader_program, "objects");
	if (uniform_location == -1)
		terminate("objects not found in shader program", 1, rt);
	glUniform1i(uniform_location, 1);
	// glBindBuffer(GL_TEXTURE_BUFFER, 0);	// unbind
}

void	bind_lights(GLuint shader_program, t_rt *rt)
{
	GLint uniform_location;

	// glBindBuffer(GL_TEXTURE_BUFFER, rt->tbo_lights_id);
	// glActiveTexture(GL_TEXTURE2);
	// glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, rt->tbo_lights_id);
	(void)rt;
	uniform_location = glGetUniformLocation(shader_program, "lights");
	if (uniform_location == -1)
		terminate("lights not found in shader program", 1, rt);
	glUniform1i(uniform_location, 2);
	// glBindBuffer(GL_TEXTURE_BUFFER, 0);	// unbind
}

void	bind_agx_lut(GLuint shader_program, t_rt *rt)
{
	GLint uniform_location;

	// glBindBuffer(GL_TEXTURE_BUFFER, rt->tbo_agx_lut_id);
	// glActiveTexture(GL_TEXTURE3);
	// glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, rt->tbo_agx_lut_id);
	(void)rt;
	uniform_location = glGetUniformLocation(shader_program, "agx_lut");
	if (uniform_location == -1)
		terminate("agx_lut not found in shader program", 1, rt);
	glUniform1i(uniform_location, 3);
	// glBindBuffer(GL_TEXTURE_BUFFER, 0);	// unbind
}

void	bind_environment_map(GLuint shader_program, t_rt *rt)
{
	GLint uniform_location;

	(void)rt;
	uniform_location = glGetUniformLocation(shader_program, "environment_map");
	if (uniform_location == -1)
		terminate("environment_map not found in shader program", 1, rt);
	glUniform1i(uniform_location, 4);
}
