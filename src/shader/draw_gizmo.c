/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_gizmo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 16:37:19 by bwerner           #+#    #+#             */
/*   Updated: 2024/10/31 07:18:59 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

static double	dvec2_distance(t_dvec2 a, t_dvec2 b)
{
	double	distance;

	distance = sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
	return (distance);
}

static bool	gizmo_is_selected(t_rt *rt)
{
	t_dvec2	gizmo_pos;

	gizmo_pos.x = rt->width / rt->dpi_scale - 50;
	gizmo_pos.y = 50;
	if (dvec2_distance(rt->cursor_pos, gizmo_pos) <= 40)
		return (true);
	return (false);
}

static void	bind_uniforms_1(t_rt *rt)
{
	GLint	pitch;
	GLint	yaw;
	GLint	aspect;
	GLint	scale;
	GLint	dpi_scale;

	pitch = glGetUniformLocation(rt->gizmo_shader_program, "u_pitch");
	yaw = glGetUniformLocation(rt->gizmo_shader_program, "u_yaw");
	aspect = glGetUniformLocation(rt->gizmo_shader_program, "u_aspect_ratio");
	scale = glGetUniformLocation(rt->gizmo_shader_program, "u_scale");
	dpi_scale = glGetUniformLocation(rt->gizmo_shader_program, "u_dpi_scale");
	glUniform1f(pitch, rt->camera.pitch);
	glUniform1f(yaw, -rt->camera.yaw);
	glUniform1f(aspect, (float)rt->width / rt->height);
	glUniform1f(scale, 0.6272 * rt->dpi_scale / rt->width);
	glUniform1f(dpi_scale, rt->dpi_scale);
}

static void	bind_uniforms_2(t_rt *rt)
{
	GLint	selected;
	GLint	mac_os;
	GLint	debug;

	selected = glGetUniformLocation(rt->gizmo_shader_program, "u_selected");
	mac_os = glGetUniformLocation(rt->gizmo_shader_program, "u_mac_os");
	debug = glGetUniformLocation(rt->gizmo_shader_program, "u_debug");
	glUniform1i(selected, (int)gizmo_is_selected(rt));
	glUniform1i(mac_os, (int)MAC_OS);
	glUniform1f(debug, rt->debug);
}

void	draw_gizmo(t_rt *rt)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(rt->gizmo_shader_program);
	bind_framebuffer(0, rt->gizmo_shader_program, rt);
	bind_uniforms_1(rt);
	bind_uniforms_2(rt);
	glBindVertexArray(rt->vao_gizmo_id);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_TRIANGLES, 0, 72);
	glFinish();
	glDisable(GL_BLEND);
	glDisable(GL_MULTISAMPLE);
	glDisable(GL_DEPTH_TEST);
}
