/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 18:37:08 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/25 20:37:06 by nmihaile         ###   ########.fr       */
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

void	update(t_rt *rt)
{
	static int	i;
	static double	start;
	static double	oldstart;

	start = glfwGetTime();
	rt->delta_time = start - oldstart;
	oldstart = start;

	handle_move_input(rt);
	move_camera(rt);
	// update_screen(rt);

	i++;
	if (i == 1)
		ft_timer(TIMER_START, NULL);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// if (rt->mode == MODE_SOLID)
	// 	fill_image(rt->canvas, (t_vec4){{0.25, 0.25, 0.25, 1}});
	// else if (rt->mode == MODE_NORMAL)
	// 	fill_image(rt->canvas, (t_vec4){{0, 0, 0, 1}});
	// else
	// 	fill_image(rt->canvas, rt->ambient);
	// render(rt);

	// DRAW SCREEN
	glBindVertexArray(rt->vertex_array_object);
	glDrawArrays(GL_TRIANGLES, 0, 6);


	if (i == 50)
	{
		ft_timer(TIMER_STOP, NULL);
		i = 0;
	}

	glfwSwapBuffers(rt->window);
	glfwPollEvents();
}
