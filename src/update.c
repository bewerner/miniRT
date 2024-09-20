/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 18:37:08 by nmihaile          #+#    #+#             */
/*   Updated: 2024/09/20 01:59:41 by bwerner          ###   ########.fr       */
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

	glBindBuffer(GL_UNIFORM_BUFFER, rt->ubo_rt_id);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(t_ubo), &ubo_rt);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void	update_window_title(t_rt *rt)
{
	static size_t	i;
	static float	time;
	char			title[1024];
	char			*fps;
	char			*frame;
	char			*max_samples;

	frame = NULL;
	max_samples = NULL;
	title[0] = '\0';
	i++;
	time += rt->delta_time;
	if (time < 0.5f)
		return ;
	fps = ft_itoa((1 / (time / i)));
	i = 0;
	time = 0;
	if (fps)
	{
		ft_strlcat(title, fps, 1024);
		ft_strlcat(title, " FPS - ", 1024);
	}
	ft_strlcat(title, "miniRT - ", 1024);
	ft_strlcat(title, rt->filename, 1024);
	if (rt->mode == MODE_PREVIEW)
	{
		ft_strlcat(title, " - sample ", 1024);
		frame = ft_itoa(rt->sample_count);
		if (frame)
			ft_strlcat(title, frame, 1024);
		ft_strlcat(title, "/", 1024);
		max_samples = ft_itoa(rt->max_samples);
		if (max_samples)
			ft_strlcat(title, max_samples, 1024);
	}
	glfwSetWindowTitle(rt->window, title);
	free(fps);
	free(frame);
	free(max_samples);
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
	update_ubo_rt(rt);
	update_window_title(rt);
	if (rt->mode == MODE_PREVIEW && rt->sample_count < rt->max_samples)
	{
		rt->sample_count++;
		if (rt->sample_count <= 1)
		glfwSwapInterval(0);
	}
	else
		glfwSwapInterval(SWAP_INTERVAL);






	// glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	// glClear(GL_COLOR_BUFFER_BIT);

	render_raw_image(rt);
	postprocess_raw_image(rt);
	
	if (rt->mode != MODE_PREVIEW)
		draw_gizmo(rt);

	// DISPLAY DEFAULT FRAMEBUFFER (postprocessed image with gizmo)
	glfwSwapBuffers(rt->window);
	glfwPollEvents();








	if (i == 60)
	{
		ft_timer(TIMER_STOP, NULL);
		i = 0;
	}
	// usleep(100000);
}
