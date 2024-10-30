/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hook.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 19:55:23 by bwerner           #+#    #+#             */
/*   Updated: 2024/10/30 00:41:16 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static void	print_camera_info(t_rt *rt)
{
	printf("C    %f, %f, %f    %f, %f, %f    %f\n",
		rt->camera.origin.x, rt->camera.origin.y, rt->camera.origin.z,
		rt->camera.direction.x, rt->camera.direction.y, rt->camera.direction.z,
		rt->camera.fov);
}

void	reset_camera(t_camera *camera, t_rt *rt)
{
	static t_camera	initial_camera;
	static bool		initialized;

	if (initialized == false)
	{
		initial_camera = *camera;
		initialized = true;
		return ;
	}
	*camera = initial_camera;
	rt->move.vel = (t_vec3){{0, 0, 0}};
}

static void	key_hook_axial_view(int key, int action, t_rt *rt)
{
	if (key == GLFW_KEY_KP_1 && action == GLFW_PRESS)
	{
		rt->sample_count = 0;
		rt->camera.origin = (t_vec3){{0, -vec3_len(rt->camera.origin), 0}};
		rt->camera.yaw = 0;
		rt->camera.pitch = M_PI / 2;
		rt->move.vel = (t_vec3){{0, 0, 0}};
	}
	else if (key == GLFW_KEY_KP_3 && action == GLFW_PRESS)
	{
		rt->sample_count = 0;
		rt->camera.origin = (t_vec3){{vec3_len(rt->camera.origin), 0, 0}};
		rt->camera.yaw = M_PI / 2;
		rt->camera.pitch = M_PI / 2;
		rt->move.vel = (t_vec3){{0, 0, 0}};
	}
	else if (key == GLFW_KEY_KP_7 && action == GLFW_PRESS)
	{
		rt->sample_count = 0;
		rt->camera.origin = (t_vec3){{0, 0, vec3_len(rt->camera.origin)}};
		rt->camera.yaw = 0;
		rt->camera.pitch = 0;
		rt->move.vel = (t_vec3){{0, 0, 0}};
	}
}

void	check_debug_key_presses(int key, int action, t_rt *rt)
{
	if (key == GLFW_KEY_UP && action)
	{
		rt->debug += 1;
		rt->sample_count = 0;
		printf("debug is %f\n", rt->debug);
	}
	else if (key == GLFW_KEY_DOWN && action)
	{
		rt->debug -= 1;
		rt->sample_count = 0;
		printf("debug is %f\n", rt->debug);
	}
	if (key == GLFW_KEY_RIGHT && action)
	{
		rt->debug2 += 1;
		rt->sample_count = 0;
		printf("debug2 is %f\n", rt->debug2);
	}
	else if (key == GLFW_KEY_LEFT && action)
	{
		rt->debug2 -= 1;
		rt->sample_count = 0;
		printf("debug2 is %f\n", rt->debug2);
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
		reset_camera(&rt->camera, rt);
	else if (key == GLFW_KEY_G && action == GLFW_PRESS)
		rt->hide_gizmo = !rt->hide_gizmo;
	else if (key == GLFW_KEY_P && action == GLFW_PRESS)
		print_camera_info(rt);
	else
		key_hook_axial_view(key, action, rt);
	check_debug_key_presses(key, action, rt);
}
