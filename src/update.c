/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 18:37:08 by nmihaile          #+#    #+#             */
/*   Updated: 2024/09/28 13:53:40 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

void	update_ubo_rt(t_rt *rt)
{
	t_ubo	ubo_rt;

	ubo_rt.sample_count = rt->sample_count;
	ubo_rt.max_samples = rt->max_samples;
	ubo_rt.debug = rt->debug;
	ubo_rt.ambient = rt->ambient;
	ubo_rt.aspect_ratio = rt->aspect_ratio;
	ubo_rt.camera = rt->camera;
	ubo_rt.width = rt->width;
	ubo_rt.height = rt->height;
	ubo_rt.mode = (int)rt->mode;
	ubo_rt.mac_os = MAC_OS;
	glBindBuffer(GL_UNIFORM_BUFFER, rt->ubo_rt_id);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(t_ubo), &ubo_rt);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void	update(t_rt *rt)
{
	static int		i;
	static double	start;
	static double	oldstart;

	i++;
	handle_move_input(rt);
	move_camera(rt);
	if (i == 1)
		ft_timer(TIMER_START, NULL);
	// glfwSetCursorPos(rt->window, 123, 123);
	start = glfwGetTime();
	rt->delta_time = start - oldstart;
	oldstart = start;
	if (rt->mode == MODE_PREVIEW && rt->sample_count <= rt->max_samples)
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
		postprocess_raw_image(rt);
		if (rt->mode != MODE_PREVIEW)
			draw_gizmo(rt);
		glfwSwapBuffers(rt->window);
	}
	else
		usleep(1000000/60);

	glfwPollEvents();

	if (i == 60)
	{
		ft_timer(TIMER_STOP, NULL);
		i = 0;
	}
}
