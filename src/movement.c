/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 16:32:57 by nmihaile          #+#    #+#             */
/*   Updated: 2024/11/14 08:12:08 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

void	move_camera(t_rt *rt)
{
	t_camera	initial_camera;

	initial_camera = rt->camera;
	rt->camera.right = (t_vec3){{1, 0, 0}};
	rt->camera.right = vec3_rotate_z(rt->camera.right, rt->camera.yaw);
	rt->camera.up = vec3_cross(rt->camera.direction, rt->camera.right);
	rt->camera.direction = (t_vec3){{0, 0, -1}};
	rt->camera.direction = vec3_rotate_x(rt->camera.direction, rt->camera.pitch);
	rt->camera.direction = vec3_rotate_z(rt->camera.direction, rt->camera.yaw);
	rt->move.vel = rt->move.acc;
	rt->camera.origin = vec3_add(rt->camera.origin, vec3_scale(rt->move.vel.x, rt->camera.right));
	rt->camera.origin = vec3_add(rt->camera.origin, (t_vec3){{0, 0, rt->move.vel.y}});
	rt->camera.origin = vec3_add(rt->camera.origin, vec3_scale(rt->move.vel.z, rt->camera.direction));
	rt->camera.viewport_light = (t_vec3){{-0.348155, 0.348155, 0.870388}};
	rt->camera.viewport_light = vec3_rotate_x(rt->camera.viewport_light, rt->camera.pitch);
	rt->camera.viewport_light = vec3_rotate_z(rt->camera.viewport_light, rt->camera.yaw);
	if (ft_memcmp(&initial_camera, &rt->camera, sizeof(t_camera)))
	{
		rt->moving = true;
		rt->sample_count = 0;
	}
	else
		rt->moving = false;
}

void	handle_movement_input(t_rt *rt)
{
	rt->move.acc = (t_vec3){{0, 0, 0}};
	if (glfwGetKey(rt->window, GLFW_KEY_A))
		rt->move.acc.x -= ACC * fminf(rt->delta_time, 0.1);
	if (glfwGetKey(rt->window, GLFW_KEY_D))
		rt->move.acc.x += ACC * fminf(rt->delta_time, 0.1);
	if (glfwGetKey(rt->window, GLFW_KEY_W))
		rt->move.acc.z += ACC * fminf(rt->delta_time, 0.1);
	if (glfwGetKey(rt->window, GLFW_KEY_S))
		rt->move.acc.z -= ACC * fminf(rt->delta_time, 0.1);
	if (glfwGetKey(rt->window, GLFW_KEY_E) || glfwGetKey(rt->window, GLFW_KEY_SPACE))
		rt->move.acc.y += ACC * fminf(rt->delta_time, 0.1);
	if (glfwGetKey(rt->window, GLFW_KEY_C))
		rt->move.acc.y -= ACC * fminf(rt->delta_time, 0.1);
	rt->move.acc = vec3_scale(rt->move.speed, rt->move.acc);
	move_camera(rt);
}
