/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hook.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 19:55:23 by bwerner           #+#    #+#             */
/*   Updated: 2024/12/17 07:20:27 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

static t_vec3 P3_to_sRGB(t_vec3 in)
{
	t_vec3	out;
	
	out.r =  1.2249 * in.r - 0.2247 * in.g - 0.0003 * in.b;
	out.g = -0.0421 * in.r + 1.0424 * in.g - 0.0004 * in.b;
	out.b = -0.0196 * in.r - 0.0786 * in.g + 1.0984 * in.b;

	out.r = fminf(1.0, fmaxf(0.0, out.r));
	out.g = fminf(1.0, fmaxf(0.0, out.g));
	out.b = fminf(1.0, fmaxf(0.0, out.b));
	
	return (out);
}

static void	write_png(t_rt *rt)
{
	uint8_t	*image;
	char	path[1024];
	char	*scene_name;

	scene_name = ft_strrchr(rt->filename, '/');
	if (scene_name)
		scene_name += 1;
	else
		scene_name = rt->filename;

	path[0] = '\0';
	ft_strlcat(path, "images/", 1024);
	ft_strlcat(path, scene_name, 1024);
	*ft_strrchr(path, '.') = '\0';
	ft_strlcat(path, "__", 1024);
	char *samples = ft_itoa(ft_imin(rt->sample_count, rt->max_samples));
	if (!samples)
	{
		perror("Failed to allocate memory for saving render image");
		return ;
	}
	ft_strlcat(path, samples, 1024);
	free (samples);
	ft_strlcat(path, "_samples.png", 1024);

	mkdir("images", 0755);

	image = (uint8_t *)malloc(rt->width * rt->height * 3 * sizeof(uint8_t));
	if (!image)
	{
		perror("Failed to allocate memory for saving render image");
		return ;
	}
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(0, 0, rt->width, rt->height, GL_RGB, GL_UNSIGNED_BYTE, image);
	if (MAC_OS)
	{
		for (size_t i = 0; i < (size_t)rt->width * rt->height * 3; i += 3)
		{
			t_vec3 col = (t_vec3){{(float)image[i] / 255, (float)image[i+1] / 255, (float)image[i+2] / 255}};
			col = P3_to_sRGB(col);
			image[i]   = roundf(col.r * 255);
			image[i+1] = roundf(col.g * 255);
			image[i+2] = roundf(col.b * 255);
		}
	}
	stbi_flip_vertically_on_write(true);
	if (stbi_write_png(path, rt->width, rt->height, 3, image, rt->width * 3))
		printf("Saved render image: %s\n", path);
	else
		perror("Failed to save render image");
	free(image);
}

static void	print_camera_info(t_rt *rt)
{
	printf("C    %f, %f, %f    %f, %f, %f    %f",
		rt->camera.origin.x, rt->camera.origin.y, rt->camera.origin.z,
		rt->camera.direction.x, rt->camera.direction.y, rt->camera.direction.z,
		rt->camera.fov);
	if (rt->camera.f_stop > 0)
		printf("    %f    %f", rt->camera.f_stop, rt->camera.focus_distance);
	printf("\n");
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

void	key_hook_debug(int key, int action, t_rt *rt)
{
	if (key == GLFW_KEY_UP && action)
	{
		rt->debug += 1;
		rt->sample_count = 0;
		printf("debug:  %f\n", rt->debug);
	}
	else if (key == GLFW_KEY_DOWN && action)
	{
		rt->debug -= 1;
		rt->sample_count = 0;
		printf("debug:  %f\n", rt->debug);
	}
	if (key == GLFW_KEY_RIGHT && action)
	{
		rt->debug2 += 1;
		rt->sample_count = 0;
		printf("debug2: %f\n", rt->debug2);
	}
	else if (key == GLFW_KEY_LEFT && action)
	{
		rt->debug2 -= 1;
		rt->sample_count = 0;
		printf("debug2: %f\n", rt->debug2);
	}
}

void	key_hook_render_scale(int key, int action, t_rt *rt)
{
	float before = rt->max_render_scale;
	if (key == GLFW_KEY_I && action)
		rt->max_render_scale *= 2;
	else if (key == GLFW_KEY_K && action)
		rt->max_render_scale /= 2;
	else
		return ;
	rt->max_render_scale = fmaxf(1.0f / 64, rt->max_render_scale);
	rt->max_render_scale = fminf(1.0f, rt->max_render_scale);
	rt->render_scale = rt->max_render_scale;
	if (rt->max_render_scale != before)
		resize_framebuffer(rt, rt->render_scale);
	printf("Render Scale: %3.2f%%\n", 100.0f * rt->render_scale);
}

void	key_hook_mode(int key, int action, t_rt *rt)
{
	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
	{
		rt->mode++;
		if (rt->mode > MODE_RENDER)
			rt->mode = MODE_SOLID;
	}
	else if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		rt->mode = MODE_SOLID;
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
		rt->mode = MODE_NORMAL;
	else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
		rt->mode = MODE_RENDER;
	else
		return ;
	rt->sample_count = 0;
}

void	key_hook(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	t_rt	*rt;

	(void)scancode;
	rt = get_rt();
	if (((key == GLFW_KEY_ESCAPE) || (key == GLFW_KEY_Q))
		&& action == GLFW_PRESS && !mods)
		glfwSetWindowShouldClose(window, true);
	else if (key == GLFW_KEY_R && action == GLFW_PRESS)
		reset_camera(&rt->camera, rt);
	else if (key == GLFW_KEY_G && action == GLFW_PRESS)
		rt->show_gizmo = !rt->show_gizmo;
	else if (key == GLFW_KEY_P && action == GLFW_PRESS)
		print_camera_info(rt);
	else if (key == GLFW_KEY_O && action == GLFW_PRESS)
		write_png(rt);
	else if (key == GLFW_KEY_M && action == GLFW_PRESS)
		rt->cursor_is_settable = !rt->cursor_is_settable;
	else if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		rt->camera.f_stop *= -1;
		rt->sample_count = 0;
	}
	key_hook_axial_view(key, action, rt);
	key_hook_debug(key, action, rt);
	key_hook_render_scale(key, action, rt);
	key_hook_mode(key, action, rt);
}
