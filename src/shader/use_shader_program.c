/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   use_shader_program.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 16:57:50 by bwerner           #+#    #+#             */
/*   Updated: 2024/09/18 17:45:00 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

void	use_shader_program(GLuint shader_program, t_rt *rt)
{
	glUseProgram(shader_program);

	// glBindBufferBase(GL_UNIFORM_BUFFER, 0, rt->ubo_rt_id);
	// glBindBufferBase(GL_UNIFORM_BUFFER, 0, 0); // unbind

	glBindBuffer(GL_TEXTURE_BUFFER, rt->tbo_objects_id);
	glActiveTexture(GL_TEXTURE0 + 1);
	// glBindTexture(GL_TEXTURE_BUFFER, rt->objects_texture_id);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, rt->tbo_objects_id);
	GLint uniform_location = glGetUniformLocation(shader_program, "objects");
	if (uniform_location == -1)
		terminate("objects not found in shader program", 1, rt);
	glUniform1i(uniform_location, 1);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);	// unbind

	glBindBuffer(GL_TEXTURE_BUFFER, rt->tbo_lights_id);
	glActiveTexture(GL_TEXTURE0 + 2);
	// glBindTexture(GL_TEXTURE_BUFFER, rt->lights_texture_id);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, rt->tbo_lights_id);
	uniform_location = glGetUniformLocation(shader_program, "lights");
	if (uniform_location == -1)
		terminate("lights not found in shader program", 1, rt);
	glUniform1i(uniform_location, 2);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);	// unbind

	// RE-BIND AGX
	glBindBuffer(GL_TEXTURE_BUFFER, rt->tbo_agx_lut_id);
	glActiveTexture(GL_TEXTURE0 + 3);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, rt->tbo_agx_lut_id);
	uniform_location = glGetUniformLocation(shader_program, "agx_lut");
	if (uniform_location == -1)
		terminate("agx_lut not found in shader program", 1, rt);
	glUniform1i(uniform_location, 3);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);	// unbind

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rt->tex_fbo_id);
	glUniform1i(glGetUniformLocation(shader_program, "raw_render_image"), 0);  // Bind the texture to the uniform
}
