/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 16:32:57 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/03 17:05:11 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

#define ACC  0.08f
#define DAMP 0.65f

void	handle_move_input(t_rt *rt)
{
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
	rt->move.vel = vec3_add(rt->move.vel, rt->move.acc);
	rt->move.vel = vec3_scale(DAMP, rt->move.vel);
	rt->move.acc = vec3_scale(DAMP, rt->move.acc);

	rt->camera.origin = vec3_add(rt->camera.origin, vec3_scale(rt->move.vel.x, rt->screen.x));
	rt->camera.origin = vec3_add(rt->camera.origin, (t_vec3){0, 0, rt->move.vel.y});
	rt->camera.origin = vec3_add(rt->camera.origin, vec3_scale(rt->move.vel.z, rt->camera.direction));
}
