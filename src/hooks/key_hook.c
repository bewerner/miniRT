/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hook.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 19:55:23 by bwerner           #+#    #+#             */
/*   Updated: 2024/08/14 22:45:07 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

void	reset_camera(t_camera *camera)
{
	static t_camera	cam;
	static bool		is_set;

	if (is_set == false)
	{
		cam = *camera;
		is_set = true;
	}
	else
		*camera = cam;
}

static void	key_hook_axial_view(mlx_key_data_t keydata, t_rt *rt)
{
	if (keydata.key == MLX_KEY_KP_1 && keydata.action == MLX_PRESS)
	{
		rt->camera.origin = (t_vec3){0, -vec3_len(rt->camera.origin), 0};
		rt->camera.yaw = 0;
		rt->camera.pitch = M_PI / 2;
		rt->move.vel = g_vec3_zero;
	}
	else if (keydata.key == MLX_KEY_KP_3 && keydata.action == MLX_PRESS)
	{
		rt->camera.origin = (t_vec3){-vec3_len(rt->camera.origin), 0, 0};
		rt->camera.yaw = -M_PI / 2;
		rt->camera.pitch = M_PI / 2;
		rt->move.vel = g_vec3_zero;
	}
	else if (keydata.key == MLX_KEY_KP_7 && keydata.action == MLX_PRESS)
	{
		rt->camera.origin = (t_vec3){0, 0, vec3_len(rt->camera.origin)};
		rt->camera.yaw = 0;
		rt->camera.pitch = 0;
		rt->move.vel = g_vec3_zero;
	}
}

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_rt	*rt;

	rt = (t_rt *)param;
	if (((keydata.key == MLX_KEY_ESCAPE) || (keydata.key == MLX_KEY_Q))
		&& keydata.action == MLX_PRESS)
		mlx_close_window(rt->mlx);
	else if (keydata.key == MLX_KEY_TAB && keydata.action == MLX_PRESS)
	{
		rt->mode++;
		if (rt->mode > MODE_PREVIEW)
			rt->mode = MODE_SOLID;
	}
	else if (keydata.key == MLX_KEY_R && keydata.action == MLX_PRESS)
	{
		reset_camera(&rt->camera);
		rt->move.vel = g_vec3_zero;
	}
	else
		key_hook_axial_view(keydata, rt);
}
