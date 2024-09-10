/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 16:32:57 by nmihaile          #+#    #+#             */
/*   Updated: 2024/09/10 16:46:24 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

// #define ACC  0.15f
// #define DAMP 0.65f
#define ACC  4.5f
#define DAMP 0.65f

void	handle_move_input(t_rt *rt)
{
	rt->move.acc = g_vec3_zero;
	if (glfwGetKey(rt->window, GLFW_KEY_A))
		rt->move.acc.x -= ACC * rt->delta_time;
	if (glfwGetKey(rt->window, GLFW_KEY_D))
		rt->move.acc.x += ACC * rt->delta_time;
	if (glfwGetKey(rt->window, GLFW_KEY_W))
		rt->move.acc.z += ACC * rt->delta_time;
	if (glfwGetKey(rt->window, GLFW_KEY_S))
		rt->move.acc.z -= ACC * rt->delta_time;
	if (glfwGetKey(rt->window, GLFW_KEY_E)
		|| glfwGetKey(rt->window, GLFW_KEY_SPACE))
		rt->move.acc.y += ACC * rt->delta_time;
	if (glfwGetKey(rt->window, GLFW_KEY_C))
		rt->move.acc.y -= ACC * rt->delta_time;
	rt->move.acc = vec3_scale(rt->move.speed, rt->move.acc);
}

void	move_camera(t_rt *rt)
{
	rt->camera.right = (t_vec3){1, 0, 0};
	rt->camera.right = vec3_rotate_z(rt->camera.right, rt->camera.yaw);
	rt->camera.direction = (t_vec3){0, 0, -1};
	rt->camera.direction
		= vec3_rotate_x(rt->camera.direction, rt->camera.pitch);
	rt->camera.direction
		= vec3_rotate_z(rt->camera.direction, rt->camera.yaw);

	rt->move.vel = rt->move.acc;
	// rt->move.vel = vec3_add(rt->move.vel, rt->move.acc);
	// rt->move.vel = vec3_scale(DAMP, rt->move.vel);

	rt->camera.origin = vec3_add(rt->camera.origin,
			vec3_scale(rt->move.vel.x, rt->camera.right));
	rt->camera.origin = vec3_add(rt->camera.origin,
			(t_vec3){0, 0, rt->move.vel.y});
	rt->camera.origin = vec3_add(rt->camera.origin,
			vec3_scale(rt->move.vel.z, rt->camera.direction));
	rt->camera.viewport_light = (t_vec3){-0.348155, 0.348155, 0.870388};
	rt->camera.viewport_light
		= vec3_rotate_x(rt->camera.viewport_light, rt->camera.pitch);
	rt->camera.viewport_light
		= vec3_rotate_z(rt->camera.viewport_light, rt->camera.yaw);
}
