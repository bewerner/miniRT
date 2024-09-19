/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   use_shader_program.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 16:57:50 by bwerner           #+#    #+#             */
/*   Updated: 2024/09/19 23:38:23 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

void	draw_gizmo(t_rt *rt)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(rt->gizmo_shader_program);

	// Get the locations of the uniforms in the shader
	GLint gizmo_pitch = glGetUniformLocation(rt->gizmo_shader_program, "u_pitch");
	GLint gizmo_yaw = glGetUniformLocation(rt->gizmo_shader_program, "u_yaw");
	GLint gizmo_aspect = glGetUniformLocation(rt->gizmo_shader_program, "u_aspect_ratio");
	GLint gizmo_scale = glGetUniformLocation(rt->gizmo_shader_program, "u_scale");
	GLint gizmo_debug = glGetUniformLocation(rt->gizmo_shader_program, "u_debug");
	// Set the values of the uniforms
	glUniform1f(gizmo_pitch, rt->camera.pitch);
	glUniform1f(gizmo_yaw, -rt->camera.yaw);
	// printf("pitch: %f, yaw %f\n", rt->camera.pitch, rt->camera.yaw);
	glUniform1f(gizmo_aspect, (float)rt->width / rt->height);
	glUniform1f(gizmo_scale, 0.6272 * rt->dpi_scale / rt->width);
	glUniform1f(gizmo_debug, rt->debug);

	glBindVertexArray(rt->vao_gizmo_id);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_TRIANGLES, 0, 66);
	glDisable(GL_BLEND);
	glDisable(GL_MULTISAMPLE);
	glDisable(GL_DEPTH_TEST);
}

void	use_shader_program(GLuint shader_program, t_rt *rt)
{
	glUseProgram(shader_program);

	// glBindBufferBase(GL_UNIFORM_BUFFER, 0, rt->ubo_rt_id);
	// glBindBufferBase(GL_UNIFORM_BUFFER, 0, 0); // unbind

	// FBO Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rt->tex_fbo_id);
	glUniform1i(glGetUniformLocation(shader_program, "raw_render_image"), 0);

	// OBJECTS
	glBindBuffer(GL_TEXTURE_BUFFER, rt->tbo_objects_id);
	glActiveTexture(GL_TEXTURE1);
	// glBindTexture(GL_TEXTURE_BUFFER, rt->objects_texture_id);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, rt->tbo_objects_id);
	GLint uniform_location = glGetUniformLocation(shader_program, "objects");
	if (uniform_location == -1)
		terminate("objects not found in shader program", 1, rt);
	glUniform1i(uniform_location, 1);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);	// unbind

	// LIGHTS
	glBindBuffer(GL_TEXTURE_BUFFER, rt->tbo_lights_id);
	glActiveTexture(GL_TEXTURE2);
	// glBindTexture(GL_TEXTURE_BUFFER, rt->lights_texture_id);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, rt->tbo_lights_id);
	uniform_location = glGetUniformLocation(shader_program, "lights");
	if (uniform_location == -1)
		terminate("lights not found in shader program", 1, rt);
	glUniform1i(uniform_location, 2);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);	// unbind

	// RE-BIND AGX
	glBindBuffer(GL_TEXTURE_BUFFER, rt->tbo_agx_lut_id);
	glActiveTexture(GL_TEXTURE3);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, rt->tbo_agx_lut_id);
	uniform_location = glGetUniformLocation(shader_program, "agx_lut");
	if (uniform_location == -1)
		terminate("agx_lut not found in shader program", 1, rt);
	glUniform1i(uniform_location, 3);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);	// unbind
}
