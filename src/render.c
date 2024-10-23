/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 01:52:08 by bwerner           #+#    #+#             */
/*   Updated: 2024/10/23 19:41:58 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

void	postprocess_raw_image(t_rt *rt)
{
	GLuint	shader_program;

	shader_program = rt->postprocessing_shader_program;
	glUseProgram(shader_program);
	bind_framebuffer(0, shader_program, rt);
	bind_agx_lut(shader_program, rt);
	glBindVertexArray(rt->vao_screen_id);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glFinish();
}

// fix resize hook
void	render_bounces(GLuint shader_program, t_rt *rt)
{
	while (rt->mode == MODE_PREVIEW
		&& rt->diffuse_bounce_count < rt->max_diffuse_bounces)
	{
		rt->diffuse_bounce_count++;
		bind_framebuffer(rt->fbo_id, shader_program, rt);
		update_ubo_rt(rt);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glFinish();
	}
	while (rt->mode == MODE_PREVIEW
		&& rt->glossy_bounce_count < rt->max_glossy_bounces)
	{
		rt->glossy_bounce_count++;
		bind_framebuffer(rt->fbo_id, shader_program, rt);
		update_ubo_rt(rt);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glFinish();
	}
	rt->diffuse_bounce_count = 0;
	rt->glossy_bounce_count = 0;
}

void	render_raw_image(t_rt *rt)
{
	GLuint	shader_program;

	shader_program = rt->solid_shader_program;
	if (rt->mode == MODE_NORMAL)
		shader_program = rt->normal_shader_program;
	else if (rt->mode == MODE_PREVIEW)
		shader_program = rt->preview_shader_program;
	glUseProgram(shader_program);
	bind_framebuffer(rt->fbo_id, shader_program, rt);
	bind_objects(shader_program, rt);
	if (rt->mode == MODE_PREVIEW)
		bind_environment_map(shader_program, rt);
	if (rt->mode == MODE_PREVIEW || rt->mode == MODE_SOLID)
		bind_lights(shader_program, rt);
	if (rt->mode == MODE_PREVIEW || rt->mode == MODE_NORMAL)
	{
		bind_texture_units(shader_program, rt);
		bind_material_ubo(shader_program, rt);
		bind_texture_ubo(shader_program, rt);
	}
	glBindVertexArray(rt->vao_screen_id);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glFinish();
	if (rt->max_diffuse_bounces || rt->max_glossy_bounces)
		render_bounces(shader_program, rt);
}
