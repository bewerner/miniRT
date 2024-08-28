/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_hook.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 19:56:06 by bwerner           #+#    #+#             */
/*   Updated: 2024/08/28 16:40:42 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static void	set_rotation(t_vec2 distance, t_rt *rt)
{
	rt->camera.yaw -= (float)distance.x / 700;
	if (rt->camera.yaw > 2 * M_PI)
		rt->camera.yaw -= 2 * M_PI;
	if (rt->camera.yaw < 0)
		rt->camera.yaw += 2 * M_PI;
	rt->camera.pitch -= (float)distance.y / 700;
	rt->camera.pitch = fmaxf(fminf(rt->camera.pitch, M_PI), 0);
}

void	cursor_hook(GLFWwindow* window, double cursor_x, double cursor_y)
{
	t_rt	*rt;
	t_vec2	distance;

	rt = get_rt();
	rt->clicked = 0;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) != GLFW_PRESS)
		return ;
	// if (!mlx_is_mouse_down(rt->mlx, MLX_MOUSE_BUTTON_LEFT)
	// 	&& !mlx_is_mouse_down(rt->mlx, MLX_MOUSE_BUTTON_MIDDLE)
	// 	&& !mlx_is_mouse_down(rt->mlx, MLX_MOUSE_BUTTON_RIGHT))
	// 	return ;
	distance.x = cursor_x - rt->initial_cursor_pos.x;
	distance.y = cursor_y - rt->initial_cursor_pos.y;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		set_rotation(distance, rt);
	if (rt->cursor_is_settable)
		glfwSetCursorPos(window, rt->initial_cursor_pos.x, rt->initial_cursor_pos.y);
	else
		rt->initial_cursor_pos = (t_vec2){cursor_x, cursor_y};
}
