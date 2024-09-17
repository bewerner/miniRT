/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 18:37:08 by nmihaile          #+#    #+#             */
/*   Updated: 2024/09/17 04:24:17 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

// static void	update_screen(t_rt *rt)
// {
// 	float		half_w;
// 	float		half_h;
// 	t_screen	*screen;

// 	half_w = (rt->width - 1) / 2;
// 	half_h = (rt->height - 1) / 2;
// 	screen = &rt->screen;
// 	screen->x_dir = rt->camera.right;
// 	screen->y_dir = vec3_cross(rt->camera.direction, rt->camera.right);
// 	screen->origin = vec3_scale(rt->camera.focal_lenth, rt->camera.direction);
// 	screen->pos_null = screen->origin;
// 	screen->pos_null
// 		= vec3_sub(screen->pos_null, vec3_scale(half_w, screen->x_dir));
// 	screen->pos_null
// 		= vec3_sub(screen->pos_null, vec3_scale(half_h, screen->y_dir));
// }

void	update_ubo_rt(t_rt *rt)
{
	t_ubo	ubo_rt;

	ubo_rt.frame = rt->frame;
	ubo_rt.max_samples = rt->max_samples;
	ubo_rt.debug = rt->debug;
	ubo_rt.ambient = rt->ambient;
	glfwGetFramebufferSize(rt->window, &rt->width, &rt->height);
	ubo_rt.aspect_ratio = (float)rt->width / (float)rt->height;
	ubo_rt.camera = rt->camera;

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
		frame = ft_itoa(rt->frame);
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
	static int	i;
	static double	start;
	static double	oldstart;

	i++;
	handle_move_input(rt);
	move_camera(rt);
	glfwSwapInterval(1);
	if (rt->mode == MODE_PREVIEW && rt->frame < rt->max_samples)
	{
		rt->frame++;
		if (rt->frame <= 1)
		glfwSwapInterval(0);
	}
	if (i == 1)
		ft_timer(TIMER_START, NULL);
	// glfwSetCursorPos(rt->window, 123, 123);
	start = glfwGetTime();
	rt->delta_time = start - oldstart;
	oldstart = start;
	update_ubo_rt(rt);
	update_window_title(rt);

	// rt->debug = !rt->debug;
	// if (rt->frame > 4)
	// 	rt->frame = 0;
	// if (rt->frame % 5 == 1)
	// if (rt->debug == 1)
	if (1 == 1)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, rt->framebuffer);
		// glViewport(0, 0, rt->width * 1, rt->height * 1);
		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// // Activate the shader program
		// glUseProgram(rt->shader_program);

		// Bind the texture from the previous frame
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, rt->frameTexture);
		glUniform1i(glGetUniformLocation(rt->shader_program, "prevFrameTexture"), 0);  // Bind the texture to the uniform
	}


		// glBindFramebuffer(GL_FRAMEBUFFER, rt->framebuffer);
		// glViewport(0, 0, rt->width * 1, rt->height * 1);
		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		// glClear(GL_COLOR_BUFFER_BIT);

		// // Activate the shader program
		// glUseProgram(rt->shader_program);

		// // Bind the texture from the previous frame
		// glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_2D, rt->frameTexture);
		// glUniform1i(glGetUniformLocation(rt->shader_program, "prevFrameTexture"), 0);  // Bind the texture to the uniform





		// DRAW SCREEN
		glBindVertexArray(rt->vertex_array_object);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	glFinish();

		// After rendering to the texture, unbind the framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (rt->debug >= -1)
		rt->debug = -1;

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Activate the shader program
		glUseProgram(rt->shader_program);

		// Bind the texture from the previous frame
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, rt->frameTexture);
		glUniform1i(glGetUniformLocation(rt->shader_program, "prevFrameTexture"), 0);  // Bind the texture to the uniform

		// DRAW SCREEN
		glBindVertexArray(rt->vertex_array_object);
		glDrawArrays(GL_TRIANGLES, 0, 6);

	glFinish();
		
	glfwSwapBuffers(rt->window);
	glfwPollEvents();
	if (rt->debug >= -1)
		rt->debug = 0;

	// sleep(1);
	if (i == 60)
	{
		ft_timer(TIMER_STOP, NULL);
		i = 0;
	}
	// usleep(100000);
}
