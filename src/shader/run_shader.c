/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_shader.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 01:52:08 by bwerner           #+#    #+#             */
/*   Updated: 2024/09/20 19:54:09 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static double	dvec2_distance(t_dvec2 a, t_dvec2 b)
{
	double	distance;

	distance = sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
	return (distance);
}

bool	gizmo_is_selected(t_rt *rt)
{
	t_dvec2	gizmo_pos;

	gizmo_pos.x = rt->width / rt->dpi_scale - 50;
	gizmo_pos.y = 50;

	if (dvec2_distance(rt->cursor_pos, gizmo_pos) <= 40)
		return (true);
	return (false);
}

void	draw_gizmo(t_rt *rt)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(rt->gizmo_shader_program);

	// USE DEFAULT FRAMEBUFFER
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Get the locations of the uniforms in the shader
	GLint gizmo_pitch = glGetUniformLocation(rt->gizmo_shader_program, "u_pitch");
	GLint gizmo_yaw = glGetUniformLocation(rt->gizmo_shader_program, "u_yaw");
	GLint gizmo_aspect = glGetUniformLocation(rt->gizmo_shader_program, "u_aspect_ratio");
	GLint gizmo_scale = glGetUniformLocation(rt->gizmo_shader_program, "u_scale");
	GLint gizmo_dpi_scale = glGetUniformLocation(rt->gizmo_shader_program, "u_dpi_scale");
	GLint gizmo_selected = glGetUniformLocation(rt->gizmo_shader_program, "u_selected");
	GLint gizmo_debug = glGetUniformLocation(rt->gizmo_shader_program, "u_debug");
	// Set the values of the uniforms
	glUniform1f(gizmo_pitch, rt->camera.pitch);
	glUniform1f(gizmo_yaw, -rt->camera.yaw);
	// printf("pitch: %f, yaw %f\n", rt->camera.pitch, rt->camera.yaw);
	glUniform1f(gizmo_aspect, (float)rt->width / rt->height);
	glUniform1f(gizmo_scale, 0.6272 * rt->dpi_scale / rt->width);
	glUniform1f(gizmo_dpi_scale, rt->dpi_scale);
	glUniform1i(gizmo_selected, (int)gizmo_is_selected(rt));
	glUniform1f(gizmo_debug, rt->debug);

	glBindVertexArray(rt->vao_gizmo_id);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_TRIANGLES, 0, 72);
	glDisable(GL_BLEND);
	glDisable(GL_MULTISAMPLE);
	glDisable(GL_DEPTH_TEST);
}

void	postprocess_raw_image(t_rt *rt)
{
	GLuint	shader_program;

	// USE POSTPROCESSING SHADER PROGRAM
	shader_program = rt->postprocessing_shader_program;
	glUseProgram(shader_program);

	// USE DEFAULT FRAMEBUFFER
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// ACTIVATE BUFFERS
	activate_framebuffer_texture(shader_program, rt);
	activate_agx_lut(shader_program, rt);

	// RUN SHADER
	glBindVertexArray(rt->vao_screen_id);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glFinish();
}

void	render_raw_image(t_rt *rt)
{
	GLuint	shader_program;

	// CHOOSE RENDER SHADER PROGRAM
	shader_program = rt->solid_shader_program;
	if (rt->mode == MODE_NORMAL)
		shader_program = rt->normal_shader_program;
	else if (rt->mode == MODE_PREVIEW)
		shader_program = rt->preview_shader_program;
	glUseProgram(shader_program);

	// USE OUR FRAMEBUFFER
	glBindFramebuffer(GL_FRAMEBUFFER, rt->fbo_id);

	// ACTIVATE BUFFERS
	activate_framebuffer_texture(shader_program, rt);
	activate_objects(shader_program, rt);
	if (rt->mode != MODE_NORMAL)
		activate_lights(shader_program, rt);

	// RUN SHADER
	glBindVertexArray(rt->vao_screen_id);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glFinish();
}
