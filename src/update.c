/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 18:37:08 by nmihaile          #+#    #+#             */
/*   Updated: 2024/09/10 19:52:48 by bwerner          ###   ########.fr       */
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

	ubo_rt.debug = rt->debug;
	ubo_rt.ambient = rt->ambient;
	glfwGetWindowSize(rt->window, &rt->width, &rt->height);
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

	title[0] = '\0';
	i++;
	time += rt->delta_time;
	if (time < 0.5f)
		return ;
	fps = ft_itoa((1 / (time / i)));
	if (fps)
	{
		ft_strlcat(title, fps, 1024);
		ft_strlcat(title, " FPS - ", 1024);
	}
	ft_strlcat(title, "miniRT - ", 1024);
	ft_strlcat(title, rt->filename, 1024);
	if (rt->mode == MODE_PREVIEW)
	{
		ft_strlcat(title, " - sample 1/1", 1024);
	}
	glfwSetWindowTitle(rt->window, title);
	free(fps);
	i = 0;
	time = 0;
}

void	update(t_rt *rt)
{
	static int	i;
	static double	start;
	static double	oldstart;

	i++;
	if (i == 1)
		ft_timer(TIMER_START, NULL);

	// glfwSetCursorPos(rt->window, 123, 123);
	start = glfwGetTime();
	rt->delta_time = start - oldstart;
	oldstart = start;
	update_window_title(rt);

	handle_move_input(rt);
	move_camera(rt);
	// update_screen(rt);


	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	update_ubo_rt(rt);

	// if (rt->mode == MODE_SOLID)
	// 	fill_image(rt->canvas, (t_vec3){{0.25, 0.25, 0.25, 1}});
	// else if (rt->mode == MODE_NORMAL)
	// 	fill_image(rt->canvas, (t_vec3){{0, 0, 0, 1}});
	// else
	// 	fill_image(rt->canvas, rt->ambient);
	// render(rt);

	// DRAW SCREEN
	glBindVertexArray(rt->vertex_array_object);
	glDrawArrays(GL_TRIANGLES, 0, 6);



	glFinish();
	glfwSwapBuffers(rt->window);
	glfwPollEvents();

	if (i == 60)
	{
		ft_timer(TIMER_STOP, NULL);
		i = 0;
	}
}
