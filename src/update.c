/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 18:37:08 by nmihaile          #+#    #+#             */
/*   Updated: 2024/09/18 21:35:18 by bwerner          ###   ########.fr       */
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

	if (rt->mode == MODE_SOLID)
		use_shader_program(rt->solid_shader_program, rt);
	else if (rt->mode == MODE_NORMAL)
		use_shader_program(rt->normal_shader_program, rt);
	else if (rt->mode == MODE_PREVIEW)
		use_shader_program(rt->preview_shader_program, rt);

	// USE OUR FRAMEBUFFER
	glBindFramebuffer(GL_FRAMEBUFFER, rt->fbo_id);

	// RUN SHADER
	glBindVertexArray(rt->vao_screen_id);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glFinish();






	use_shader_program(rt->postprocessing_shader_program, rt);
	// USE DEFAULT FRAMEBUFFER
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// RUN SHADER
	glBindVertexArray(rt->vao_screen_id);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glFinish();





	if (rt->mode == MODE_SOLID || rt->mode == MODE_NORMAL)
	{
		glClear(GL_DEPTH_BUFFER_BIT);
		glUseProgram(rt->gizmo_shader_program);

		// Get the locations of the uniforms in the shader
		GLint gizmo_pitch = glGetUniformLocation(rt->gizmo_shader_program, "u_pitch");
		GLint gizmo_yaw = glGetUniformLocation(rt->gizmo_shader_program, "u_yaw");
		GLint gizmo_aspect = glGetUniformLocation(rt->gizmo_shader_program, "u_aspect_ratio");
		GLint gizmo_scale = glGetUniformLocation(rt->gizmo_shader_program, "u_scale");
		// Set the values of the uniforms
		glUniform1f(gizmo_pitch, rt->camera.pitch);
		glUniform1f(gizmo_yaw, -rt->camera.yaw);
		glUniform1f(gizmo_aspect, (float)rt->width / rt->height);
		glUniform1f(gizmo_scale, 1280.00 / rt->width * 0.001);

		glBindVertexArray(rt->vao_gizmo_id);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
		glDrawArrays(GL_TRIANGLES, 0, 30);
		glDisable(GL_MULTISAMPLE);
		glDisable(GL_DEPTH_TEST);
	}



	// DISPLAY DEFAULT FRAMEBUFFER (postprocessed image)
	glfwSwapBuffers(rt->window);
	glfwPollEvents();








	if (i == 60)
	{
		ft_timer(TIMER_STOP, NULL);
		i = 0;
	}
	// usleep(100000);
}
