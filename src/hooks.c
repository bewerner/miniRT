/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:12:41 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/04 19:40:44 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

void	keyhook(mlx_key_data_t keydata, void* param)
{
	t_rt	*rt;

	rt = (t_rt*)param;
	if (((keydata.key == MLX_KEY_ESCAPE) || (keydata.key == MLX_KEY_Q)) && keydata.action == MLX_PRESS)
		mlx_close_window(rt->mlx);
}

void	update_screen(t_rt *rt)
{
	double	half_w = (rt->canvas->width - 1) / 2;
	double	half_h = (rt->canvas->height - 1) / 2;

	rt->screen.x_dir = rt->camera.right;
	rt->screen.y_dir = vec3_cross(rt->camera.direction, rt->camera.right);

	rt->screen.origin = vec3_scale(rt->camera.focal_lenth, rt->camera.direction);
	rt->screen.pos_null = rt->screen.origin;
	rt->screen.pos_null = vec3_sub(rt->screen.pos_null, vec3_scale(half_w, rt->screen.x_dir));
	rt->screen.pos_null = vec3_sub(rt->screen.pos_null, vec3_scale(half_h, rt->screen.y_dir));
}

void	update(void *param)
{
	t_rt	*rt;

	rt = (t_rt*)param;

	handle_move_input(rt);
	move_camera(rt);

	update_screen(rt);

	ft_timer(TIMER_START, NULL);
	ft_memset_int(rt->canvas->pixels, 0xFF000000, rt->mlx->width * rt->mlx->height);
	render(rt);
	ft_timer(TIMER_STOP, NULL);
}

void	mouse_hook(	enum mouse_key button,		enum action action,
					enum modifier_key modifier, void *param			)
{
	t_rt	*rt;

	rt = param;
	if (action == MLX_PRESS && !modifier)
	{
		rt->mouse_buttons_pressed++;
		if (button == MLX_MOUSE_BUTTON_LEFT)
			rt->clicked = 0.3;
		mlx_get_mouse_pos(rt->mlx,
			&rt->initial_cursor_pos.x, &rt->initial_cursor_pos.y);
		mlx_set_cursor_mode(rt->mlx, MLX_MOUSE_HIDDEN);
	}
	if (action == MLX_RELEASE && !modifier)
	{
		rt->mouse_buttons_pressed = ft_imax(0, rt->mouse_buttons_pressed - 1);
		if (!rt->mouse_buttons_pressed)
		{
			mlx_set_cursor_mode(rt->mlx, MLX_MOUSE_NORMAL);
			mlx_set_mouse_pos(rt->mlx, rt->initial_cursor_pos.x,
				rt->initial_cursor_pos.y);
		}
	}
}

void	set_rotation(t_vec2 distance, t_rt *rt)
{
	rt->camera.yaw -= (double)distance.x / 700;
	if (rt->camera.yaw > 2 * M_PI)
		rt->camera.yaw -= 2 * M_PI;
	if (rt->camera.yaw < 0)
		rt->camera.yaw += 2 * M_PI;
	rt->camera.pitch -= (double)distance.y / 700;
	rt->camera.pitch = fmax(fmin(rt->camera.pitch, M_PI), 0);
	// printf("yaw:   %10f = %4.1f, pitch: %10f = %4.1f\n", rt->camera.yaw, rt->camera.yaw * 180 / M_PI, rt->camera.pitch, rt->camera.pitch * 180 / M_PI);
	// printf("yaw: %f, pitch: %f\n", rt->camera.yaw, rt->camera.pitch);
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
	mlx_set_mouse_pos(rt->mlx, rt->initial_cursor_pos.x, rt->initial_cursor_pos.y);
}

void	init_hooks(t_rt *rt)
{
	mlx_key_hook(rt->mlx, keyhook, rt);
	mlx_mouse_hook(rt->mlx, mouse_hook, rt);
	mlx_cursor_hook(rt->mlx, cursor_hook, rt);
	mlx_loop_hook(rt->mlx, update, rt);
}
