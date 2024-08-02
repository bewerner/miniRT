/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:12:41 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/02 23:32:57 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

void	keyhook(mlx_key_data_t keydata, void* param)
{
	t_rt	*rt;

	rt = (t_rt*)param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		mlx_close_window(rt->mlx);
	if (keydata.key == MLX_KEY_W && keydata.action)
		rt->camera.origin = vec3_add(rt->camera.origin, vec3_scale(0.1, rt->camera.direction));
	if (keydata.key == MLX_KEY_S && keydata.action)
		rt->camera.origin = vec3_sub(rt->camera.origin, vec3_scale(0.1, rt->camera.direction));
	if (keydata.key == MLX_KEY_A && keydata.action)
		rt->camera.origin = vec3_sub(rt->camera.origin, vec3_scale(0.1, rt->screen.x));
	if (keydata.key == MLX_KEY_D && keydata.action)
		rt->camera.origin = vec3_add(rt->camera.origin, vec3_scale(0.1, rt->screen.x));
	if (keydata.key == MLX_KEY_SPACE && keydata.action)
		rt->camera.origin = vec3_add(rt->camera.origin, vec3_scale(0.1, (t_vec3){0, 0, 1}));
	if (keydata.key == MLX_KEY_LEFT_CONTROL && keydata.action)
		rt->camera.origin = vec3_sub(rt->camera.origin, vec3_scale(0.1, (t_vec3){0, 0, 1}));
}

void	update_screen(t_rt *rt)
{
	t_vec3	forward;
	double	half_w = (rt->canvas->width - 1) / 2;
	double	half_h = (rt->canvas->height - 1) / 2;

	forward = (t_vec3){0, 0, -1};
	rt->screen.x = (t_vec3){1, 0, 0};
	rt->screen.y = (t_vec3){0, -1, 0};
	rt->screen.x = vec3_rotate_z(rt->screen.x, rt->camera.yaw);
	rt->screen.y = vec3_rotate_x(rt->screen.y, rt->camera.pitch);
	rt->screen.y = vec3_rotate_z(rt->screen.y, rt->camera.yaw);
	forward = vec3_rotate_x(forward, rt->camera.pitch);
	forward = vec3_rotate_z(forward, rt->camera.yaw);
	rt->camera.direction = forward;
	// printf("yaw: %f = %f\npitch: %f = %f\n", yaw, yaw * 180 / 3.14159, pitch, pitch * 180 / 3.14159);
	// printf("rt->camera.direction: %f, %f, %f\n", rt->camera.direction.x, rt->camera.direction.y, rt->camera.direction.z);
	// printf("forward: %f, %f, %f\n", forward.x, forward.y, forward.z);
	// printf("rt->screen.x:   %f, %f, %f\n", rt->screen.x.x, rt->screen.x.y, rt->screen.x.z);
	// printf("rt->screen.y:    %f, %f, %f\n", rt->screen.y.x, rt->screen.y.y, rt->screen.y.z);
	// exit(0);
	// printf("\n");
	rt->screen.origin = vec3_scale(rt->camera.focal_lenth, forward);
	rt->screen.pos_null = rt->screen.origin;
	rt->screen.pos_null = vec3_sub(rt->screen.pos_null, vec3_scale(half_w, rt->screen.x));
	rt->screen.pos_null = vec3_sub(rt->screen.pos_null, vec3_scale(half_h, rt->screen.y));
}

void	update(void *param)
{
	t_rt	*rt;

	rt = (t_rt*)param;
	update_screen(rt);
	render(rt);
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
	rt->camera.yaw -= (double)distance.x / 300;
	if (rt->camera.yaw > 2 * M_PI)
		rt->camera.yaw -= 2 * M_PI;
	if (rt->camera.yaw < 0)
		rt->camera.yaw += 2 * M_PI;
	rt->camera.pitch -= (double)distance.y / 300;
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
