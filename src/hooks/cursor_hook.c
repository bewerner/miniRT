/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_hook.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 19:56:06 by bwerner           #+#    #+#             */
/*   Updated: 2024/08/14 19:56:52 by bwerner          ###   ########.fr       */
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

void	cursor_hook(double cursor_x, double cursor_y, void *param)
{
	t_rt	*rt;
	t_vec2	distance;

	rt = param;
	rt->clicked = 0;
	if (!mlx_is_mouse_down(rt->mlx, MLX_MOUSE_BUTTON_LEFT)
		&& !mlx_is_mouse_down(rt->mlx, MLX_MOUSE_BUTTON_MIDDLE)
		&& !mlx_is_mouse_down(rt->mlx, MLX_MOUSE_BUTTON_RIGHT))
		return ;
	distance.x = cursor_x - rt->initial_cursor_pos.x;
	distance.y = cursor_y - rt->initial_cursor_pos.y;
	if (mlx_is_mouse_down(rt->mlx, MLX_MOUSE_BUTTON_RIGHT))
		set_rotation(distance, rt);
	mlx_set_mouse_pos(rt->mlx,
		rt->initial_cursor_pos.x, rt->initial_cursor_pos.y);
}
