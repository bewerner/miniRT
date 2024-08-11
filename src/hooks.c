/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:12:41 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/11 10:42:35 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

void	keyhook_axial_view(mlx_key_data_t keydata, t_rt *rt)
{
	if (keydata.key == MLX_KEY_KP_1 && keydata.action == MLX_PRESS)
	{
		rt->camera.origin = (t_vec3){0, -vec3_len(rt->camera.origin), 0};
		rt->camera.yaw = 0;
		rt->camera.pitch = M_PI / 2;
		rt->move.vel = VEC3_ZERO;
	}
	else if (keydata.key == MLX_KEY_KP_3 && keydata.action == MLX_PRESS)
	{
		rt->camera.origin = (t_vec3){-vec3_len(rt->camera.origin), 0, 0};
		rt->camera.yaw = -M_PI / 2;
		rt->camera.pitch = M_PI / 2;
		rt->move.vel = VEC3_ZERO;
	}
	else if (keydata.key == MLX_KEY_KP_7 && keydata.action == MLX_PRESS)
	{
		rt->camera.origin = (t_vec3){0, 0, vec3_len(rt->camera.origin)};
		rt->camera.yaw = 0;
		rt->camera.pitch = 0;
		rt->move.vel = VEC3_ZERO;
	}
}

void	keyhook(mlx_key_data_t keydata, void* param)
{
	t_rt	*rt;

	rt = (t_rt*)param;
	if (((keydata.key == MLX_KEY_ESCAPE) || (keydata.key == MLX_KEY_Q)) && keydata.action == MLX_PRESS)
		mlx_close_window(rt->mlx);
	else if (keydata.key == MLX_KEY_TAB && keydata.action == MLX_PRESS)
	{
		rt->mode++;
		if (rt->mode > MODE_PREVIEW)
			rt->mode = MODE_SOLID;
	}
	else
		keyhook_axial_view(keydata, rt);
}

void	update_screen(t_rt *rt)
{
	float	half_w = (rt->canvas->width - 1) / 2;
	float	half_h = (rt->canvas->height - 1) / 2;

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
	static int i;
	i++;

	handle_move_input(rt);
	move_camera(rt);

	update_screen(rt);

	if (i == 1)
		ft_timer(TIMER_START, NULL);
	if (rt->mode == MODE_SOLID)
		fill_image(rt->canvas, (t_vec4){{0.25, 0.25, 0.25, 1}});
	else if (rt->mode == MODE_NORMAL)
		fill_image(rt->canvas, (t_vec4){{0, 0, 0, 1}});
	else
		fill_image(rt->canvas, rt->ambient);
	render(rt);
	if (i == 50)
	{
		ft_timer(TIMER_STOP, NULL);
		i = 0;
	}
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

void	init_hooks(t_rt *rt)
{
	mlx_key_hook(rt->mlx, keyhook, rt);
	mlx_mouse_hook(rt->mlx, mouse_hook, rt);
	mlx_cursor_hook(rt->mlx, cursor_hook, rt);
	mlx_loop_hook(rt->mlx, update, rt);
}
