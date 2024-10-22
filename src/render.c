/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 01:52:08 by bwerner           #+#    #+#             */
/*   Updated: 2024/10/22 15:08:11 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

void	postprocess_raw_image(t_rt *rt)
{
	GLuint	shader_program;

	shader_program = rt->postprocessing_shader_program;
	glUseProgram(shader_program);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	bind_framebuffer_texture(shader_program, rt);
	bind_agx_lut(shader_program, rt);
	glBindVertexArray(rt->vao_screen_id);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glFinish();
}

// fix resize hook
// create a function to bind the drawbuffer and set glDrawBuffers. potentially also use it in create_fbo, or just remove glDrawBuffers from there (norminette).
void	render_raw_image(t_rt *rt)
{
	GLuint	shader_program;

	rt->max_diffuse_bounces = rt->debug; // TEMPORARY
	shader_program = rt->solid_shader_program;
	if (rt->mode == MODE_NORMAL)
		shader_program = rt->normal_shader_program;
	else if (rt->mode == MODE_PREVIEW)
		shader_program = rt->preview_shader_program;
	glUseProgram(shader_program);
	if (rt->mode == MODE_PREVIEW)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, rt->fbo_id);
		// GLenum drawBuffers[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
		// glDrawBuffers(5, drawBuffers);  // Enable drawing to all 5 color attachments
		bind_framebuffer_textures(shader_program, rt);
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, rt->fbo_id);
		// GLenum drawBuffer = GL_COLOR_ATTACHMENT0;
		// glDrawBuffers(1, &drawBuffer);
		bind_framebuffer_texture(shader_program, rt);
	}
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
	while (rt->mode == MODE_PREVIEW && rt->diffuse_bounce_count < rt->max_diffuse_bounces)
	{
		rt->diffuse_bounce_count++;
		update_ubo_rt(rt);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glFinish();
	}
	rt->diffuse_bounce_count = 0;
}
