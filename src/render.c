/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 01:52:08 by bwerner           #+#    #+#             */
/*   Updated: 2024/11/14 01:54:47 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

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

void	render_bounces(t_rt *rt)
{
	GLuint	shader_program;

	if (rt->max_diffuse_bounces)
	{
		shader_program = rt->render_diffuse_bounce_shader_program;
		glUseProgram(shader_program);
		bind_framebuffer(rt->fbo_id, shader_program, rt);
		bind_objects(shader_program, rt);
		bind_environment_map(shader_program, rt);
		bind_lights(shader_program, rt);
		bind_texture_units(shader_program, rt);
		bind_material_ubo(shader_program, rt);
		bind_texture_ubo(shader_program, rt);
	}
	while (rt->diffuse_bounce_count < rt->max_diffuse_bounces)
	{
		rt->diffuse_bounce_count++;
		update_ubo_rt(rt);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glFinish();
	}
	if (rt->max_glossy_bounces)
	{
		shader_program = rt->render_glossy_bounce_shader_program;
		glUseProgram(shader_program);
		bind_framebuffer(rt->fbo_id, shader_program, rt);
		bind_objects(shader_program, rt);
		bind_environment_map(shader_program, rt);
		bind_lights(shader_program, rt);
		bind_texture_units(shader_program, rt);
		bind_material_ubo(shader_program, rt);
		bind_texture_ubo(shader_program, rt);
	}
	while (rt->glossy_bounce_count < rt->max_glossy_bounces)
	{
		rt->glossy_bounce_count++;
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
	else if (rt->mode == MODE_RENDER)
		shader_program = rt->render_shader_program;
	glUseProgram(shader_program);
	bind_framebuffer(rt->fbo_id, shader_program, rt);
	bind_objects(shader_program, rt);
	if (rt->mode == MODE_RENDER)
		bind_environment_map(shader_program, rt);
	if (rt->mode == MODE_RENDER || rt->mode == MODE_SOLID)
		bind_lights(shader_program, rt);
	if (rt->mode == MODE_RENDER || rt->mode == MODE_NORMAL)
	{
		bind_texture_units(shader_program, rt);
		bind_material_ubo(shader_program, rt);
		bind_texture_ubo(shader_program, rt);
	}
	glBindVertexArray(rt->vao_screen_id);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glFinish();
	if (rt->mode == MODE_RENDER && (rt->max_diffuse_bounces || rt->max_glossy_bounces))
		render_bounces(rt);
	if (rt->mode == MODE_RENDER)
	{
		shader_program = rt->accumulate_shader_program;
		bind_framebuffer(rt->fbo_id, shader_program, rt);
		glUseProgram(shader_program);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glFinish();
	}
}
