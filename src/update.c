/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 18:37:08 by nmihaile          #+#    #+#             */
/*   Updated: 2024/11/14 08:40:51 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static void	update_delta_time(t_rt *rt)
{
	static bool		initialized;
	static double	start;
	static double	oldstart;
	static double	render_start;

	if (!rt->first_update_finished)
		return ;
	if (!initialized)
	{
		oldstart = glfwGetTime();
		render_start = oldstart;
		initialized = true;
		printf("Loading time was %.0f seconds\n", render_start);
		return ;
	}
	start = glfwGetTime();
	rt->delta_time = start - oldstart;
	oldstart = start;
	if (rt->sample_count == 0)
	{
		rt->render_time = 0;
		render_start = start;
	}
	else if (rt->sample_count < rt->max_samples)
		rt->render_time = start - render_start;
}

void	update_ubo_rt(t_rt *rt)
{
	t_ubo	ubo_rt;

	ubo_rt.sample_count = rt->sample_count;
	ubo_rt.max_samples = rt->max_samples;
	ubo_rt.debug = rt->debug;
	ubo_rt.debug2 = rt->debug2;
	ubo_rt.ambient = rt->ambient;
	ubo_rt.aspect_ratio = rt->aspect_ratio;
	ubo_rt.camera = rt->camera;
	ubo_rt.width = rt->width * rt->render_scale;
	ubo_rt.height = rt->height * rt->render_scale;
	ubo_rt.mode = (int)rt->mode;
	ubo_rt.mac_os = MAC_OS;
	ubo_rt.diffuse_bounce_count = rt->diffuse_bounce_count;
	ubo_rt.max_diffuse_bounces = rt->max_diffuse_bounces;
	ubo_rt.glossy_bounce_count = rt->glossy_bounce_count;
	ubo_rt.max_glossy_bounces = rt->max_glossy_bounces;
	glBindBuffer(GL_UNIFORM_BUFFER, rt->ubo_rt_id);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(t_ubo), &ubo_rt);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void	update_render_scale(t_rt *rt)
{
	if (rt->moving)
	{
		if (rt->delta_time > 0.12f)
		{
			rt->render_scale *= sqrtf(0.1f / rt->delta_time);
			rt->render_scale = fminf(rt->render_scale, rt->max_render_scale);
			resize_framebuffer(rt, rt->render_scale);
		}
	}
	else if (rt->render_scale != rt->max_render_scale)
	{
		rt->render_scale *= 2;
		rt->render_scale = fminf(rt->render_scale, rt->max_render_scale);
		resize_framebuffer(rt, rt->render_scale);
		rt->sample_count = 0;
	}
}

void	update(t_rt *rt)
{
	handle_movement_input(rt);
	update_delta_time(rt);
	update_render_scale(rt);
	glViewport(0, 0, (rt->width * rt->render_scale), (rt->height * rt->render_scale));
	if (rt->mode == MODE_RENDER && rt->sample_count <= rt->max_samples)
	{
		glfwSwapInterval(0);
		rt->sample_count++;
	}
	else
		glfwSwapInterval(1);
	update_ubo_rt(rt);
	update_window_title(rt);
	if (rt->sample_count <= rt->max_samples)
	{
		render_raw_image(rt);
		glViewport(0, 0, rt->width, rt->height);
		postprocess_raw_image(rt);
		if (!rt->hide_gizmo || rt->mode != MODE_RENDER)
			draw_gizmo(rt);
		glfwSwapBuffers(rt->window);
	}
	else
		usleep(1000000 / 30);
	glfwPollEvents();
	rt->first_update_finished = true;
}
