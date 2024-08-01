/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:12:41 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/01 03:13:29 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

void	keyhook(mlx_key_data_t keydata, void* param)
{
	t_rt	*rt;

	rt = (t_rt*)param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		mlx_close_window(rt->mlx);
	if (keydata.key == MLX_KEY_W && keydata.action == MLX_PRESS)
	{
		rt->camera.pitch += 0.1;
		rt->camera.direction = vec3_rotate_y(rt->camera.direction, 0.1);
	}
	if (keydata.key == MLX_KEY_S && keydata.action == MLX_PRESS)
	{
		rt->camera.pitch -= 0.1;
		rt->camera.direction = vec3_rotate_y(rt->camera.direction, -0.1);
	}
	if (keydata.key == MLX_KEY_A && keydata.action == MLX_PRESS)
	{
		rt->camera.direction = vec3_rotate_z(rt->camera.direction, 0.1);
		rt->camera.yaw += 0.1;
	}
	if (keydata.key == MLX_KEY_D && keydata.action == MLX_PRESS)
	{
		rt->camera.direction = vec3_rotate_z(rt->camera.direction, -0.1);
		rt->camera.yaw -= 0.1;
	}
}

void	update(void *param)
{
	t_rt	*rt;

	rt = (t_rt*)param;
	render(rt);
}

void	init_hooks(t_rt *rt)
{
	mlx_key_hook(rt->mlx, keyhook, rt);
	mlx_loop_hook(rt->mlx, update, rt);
}
