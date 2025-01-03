/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_hook.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 19:56:06 by bwerner           #+#    #+#             */
/*   Updated: 2024/10/31 07:18:59 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

static void	set_rotation(t_dvec2 distance, t_rt *rt)
{
	rt->camera.yaw -= (float)(distance.x / 700.0f);
	if (rt->camera.yaw > 2 * M_PI)
		rt->camera.yaw -= 2 * M_PI;
	if (rt->camera.yaw < 0)
		rt->camera.yaw += 2 * M_PI;
	rt->camera.pitch -= (float)(distance.y / 700.0f);
	rt->camera.pitch = fmaxf(fminf(rt->camera.pitch, M_PI), 0);
}

void	cursor_hook(GLFWwindow *window, double cursor_x, double cursor_y)
{
	t_rt	*rt;
	t_dvec2	distance;

	rt = get_rt();
	rt->cursor_pos = (t_dvec2){cursor_x, cursor_y};
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) != GLFW_PRESS)
		return ;
	distance.x = (cursor_x - rt->initial_cursor_pos.x);
	distance.y = (cursor_y - rt->initial_cursor_pos.y);
	set_rotation(distance, rt);
	rt->initial_cursor_pos = (t_dvec2){cursor_x, cursor_y};
}
