/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_miniRT.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 20:55:35 by bwerner           #+#    #+#             */
/*   Updated: 2024/08/14 23:48:52 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

static void	init_mlx_images(t_rt *rt)
{
	rt->canvas = mlx_new_image(rt->mlx, rt->mlx->width, rt->mlx->height);
	if (!rt->canvas || mlx_image_to_window(rt->mlx, rt->canvas, 0, 0) < 0)
		terminate("mlx_new_image failed", 1, rt);
}

static void	init_mlx(t_rt *rt)
{
	t_ivec2	window_size;
	t_ivec2	screen_size;
	float	ratio;

	window_size = (t_ivec2){WINDOW_WIDTH, WINDOW_HEIGHT};
	rt->mlx = mlx_init(window_size.x, window_size.y, "miniRT", 0);
	if (!rt->mlx)
		terminate("mlx_init failed", 1, rt);
	mlx_get_monitor_size(0, &screen_size.x, &screen_size.y);
	ratio = (float)window_size.x / window_size.y;
	if (window_size.x > screen_size.x)
	{
		window_size.x = screen_size.x * 0.9;
		window_size.y = (float)window_size.x / ratio;
	}
	if (window_size.y > screen_size.y)
	{
		window_size.y = screen_size.y * 0.9;
		window_size.x = (float)window_size.y * ratio;
	}
	mlx_set_window_size(rt->mlx, window_size.x, window_size.y);
	mlx_set_window_pos(rt->mlx, ((screen_size.x - rt->mlx->width) / 2),
		((screen_size.y - rt->mlx->height) / 2));
}

static void	init_camera(t_camera *camera, t_rt *rt)
{
	t_vec3	dir;
	float	rad;

	camera->direction = vec3_normalize(camera->direction);
	dir = camera->direction;
	camera->pitch = atan2f(sqrtf(dir.x * dir.x + dir.y * dir.y), -dir.z);
	camera->yaw = atan2f(dir.x, dir.y) * -1;
	rad = camera->fov * (M_PI / 180);
	camera->focal_lenth = ((float)rt->canvas->width * 0.5) / tan(rad * 0.5);
	rt->camera.right = vec3_rotate_z((t_vec3){1, 0, 0}, rt->camera.yaw);
}

static void	init_screen(t_rt *rt)
{
	float		half_w;
	float		half_h;
	t_screen	*screen;

	half_w = (rt->canvas->width - 1) / 2;
	half_h = (rt->canvas->height - 1) / 2;
	screen = &rt->screen;
	screen->x_dir = rt->camera.right;
	screen->y_dir = vec3_cross(rt->camera.direction, rt->camera.right);
	screen->origin = vec3_scale(rt->camera.focal_lenth, rt->camera.direction);
	screen->pos_null = screen->origin;
	screen->pos_null
		= vec3_sub(screen->pos_null, vec3_scale(half_w, screen->x_dir));
	screen->pos_null
		= vec3_sub(screen->pos_null, vec3_scale(half_h, screen->y_dir));
}

void	init_mini_rt(char **argv, t_rt *rt)
{
	load_scene(argv[1], rt);
	init_mlx(rt);
	init_mlx_images(rt);
	init_camera(&rt->camera, rt);
	init_screen(rt);
	init_hooks(rt);
}
