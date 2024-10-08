/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hook.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 19:55:23 by bwerner           #+#    #+#             */
/*   Updated: 2024/10/08 03:17:30 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

void	print_camera_info(t_rt *rt)
{
	printf("C    %f, %f, %f    %f, %f, %f    %f\n",
		rt->camera.origin.x, rt->camera.origin.y, rt->camera.origin.z,
		rt->camera.direction.x, rt->camera.direction.y, rt->camera.direction.z,
		rt->camera.fov);
}

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

static void	key_hook_axial_view(int key, int action, t_rt *rt)
{
	if (key == GLFW_KEY_KP_1 && action == GLFW_PRESS)
	{
		rt->sample_count = 0;
		rt->camera.origin = (t_vec3){{0, -vec3_len(rt->camera.origin), 0}};
		rt->camera.yaw = 0;
		rt->camera.pitch = M_PI / 2;
		rt->move.vel = g_vec3_zero;
	}
	else if (key == GLFW_KEY_KP_3 && action == GLFW_PRESS)
	{
		rt->sample_count = 0;
		rt->camera.origin = (t_vec3){{vec3_len(rt->camera.origin), 0, 0}};
		rt->camera.yaw = M_PI / 2;
		rt->camera.pitch = M_PI / 2;
		rt->move.vel = g_vec3_zero;
	}
	else if (key == GLFW_KEY_KP_7 && action == GLFW_PRESS)
	{
		rt->sample_count = 0;
		rt->camera.origin = (t_vec3){{0, 0, vec3_len(rt->camera.origin)}};
		rt->camera.yaw = 0;
		rt->camera.pitch = 0;
		rt->move.vel = g_vec3_zero;
	}
}

void	check_debug_key_presses(int key, int action, t_rt *rt)
{
	if (key == GLFW_KEY_UP && action)
	{
		rt->debug += 1;
		printf("debug is %f\n", rt->debug);
	}
	else if (key == GLFW_KEY_DOWN && action)
	{
		rt->debug -= 1;
		printf("debug is %f\n", rt->debug);
	}
}

void	key_hook(GLFWwindow *window, int key, int scancode,
			int action, int mods)
{
	t_rt	*rt;

	(void)scancode;
	rt = get_rt();
	if (((key == GLFW_KEY_ESCAPE) || (key == GLFW_KEY_Q))
		&& action == GLFW_PRESS && !mods)
		glfwSetWindowShouldClose(window, true);
	else if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
	{
		rt->sample_count = 0;
		rt->mode++;
		if (rt->mode > MODE_PREVIEW)
			rt->mode = MODE_SOLID;
	}
	else if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		reset_camera(&rt->camera);
		rt->move.vel = g_vec3_zero;
	}
	else if (key == GLFW_KEY_P && action == GLFW_PRESS)
		print_camera_info(rt);
	else
		key_hook_axial_view(key, action, rt);
	check_debug_key_presses(key, action, rt);
}
