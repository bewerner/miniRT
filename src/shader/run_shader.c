/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_shader.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 01:52:08 by bwerner           #+#    #+#             */
/*   Updated: 2024/10/21 16:37:30 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

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

void	render_raw_image(t_rt *rt)
{
	GLuint	shader_program;

	shader_program = rt->solid_shader_program;
	if (rt->mode == MODE_NORMAL)
		shader_program = rt->normal_shader_program;
	else if (rt->mode == MODE_PREVIEW)
		shader_program = rt->preview_shader_program;
	glUseProgram(shader_program);
	glBindFramebuffer(GL_FRAMEBUFFER, rt->fbo_id);
	bind_framebuffer_texture(shader_program, rt);
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
}
