/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 16:32:57 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/09 17:51:45 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

#define ACC  0.15f
#define DAMP 0.65f

void	handle_move_input(t_rt *rt)
{
	rt->move.acc = VEC3_ZERO;
	if (mlx_is_key_down(rt->mlx, MLX_KEY_A))
		rt->move.acc.x -= ACC;
	if (mlx_is_key_down(rt->mlx, MLX_KEY_D))
		rt->move.acc.x += ACC;
	if (mlx_is_key_down(rt->mlx, MLX_KEY_W))
		rt->move.acc.z += ACC;
	if (mlx_is_key_down(rt->mlx, MLX_KEY_S))
		rt->move.acc.z -= ACC;
	if (mlx_is_key_down(rt->mlx, MLX_KEY_E) || mlx_is_key_down(rt->mlx, MLX_KEY_SPACE))
		rt->move.acc.y += ACC;
	if (mlx_is_key_down(rt->mlx, MLX_KEY_C))
		rt->move.acc.y -= ACC;
}

void	move_camera(t_rt *rt)
{
	rt->camera.right = (t_vec3){1, 0, 0};
	rt->camera.right = vec3_rotate_z(rt->camera.right, rt->camera.yaw);

	rt->camera.direction = (t_vec3){0, 0, -1};
	rt->camera.direction = vec3_rotate_x(rt->camera.direction, rt->camera.pitch);
	rt->camera.direction = vec3_rotate_z(rt->camera.direction, rt->camera.yaw);

	rt->move.vel = vec3_add(rt->move.vel, rt->move.acc);
	rt->move.vel = vec3_scale(DAMP, rt->move.vel);
	rt->move.vel = vec3_scale(rt->mlx->delta_time * 6, rt->move.vel);

	rt->camera.origin = vec3_add(rt->camera.origin, vec3_scale(rt->move.vel.x, rt->camera.right));
	rt->camera.origin = vec3_add(rt->camera.origin, (t_vec3){0, 0, rt->move.vel.y});
	rt->camera.origin = vec3_add(rt->camera.origin, vec3_scale(rt->move.vel.z, rt->camera.direction));
}
