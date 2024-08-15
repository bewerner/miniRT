/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_miniRT.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 20:55:35 by bwerner           #+#    #+#             */
/*   Updated: 2024/08/15 22:17:08 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

static void	init_mlx_images(t_rt *rt)
{
	rt->canvas = mlx_new_image(rt->mlx, rt->mlx->width, rt->mlx->height);
	if (!rt->canvas || mlx_image_to_window(rt->mlx, rt->canvas, 0, 0) < 0)
		terminate("mlx_new_image failed", 1, rt);
}

static void	init_mlx(char *filename, t_rt *rt)
{
	t_ivec2	window_size;
	t_ivec2	screen_size;
	float	ratio;
	char	*win_title;

	window_size = (t_ivec2){WINDOW_WIDTH, WINDOW_HEIGHT};
	win_title = ft_strjoin("miniRT - ", filename);
	if (win_title == NULL)
		terminate("fatal", 1, rt);
	rt->mlx = mlx_init(window_size.x, window_size.y, win_title, 1);
	free(win_title);
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
	init_mlx_images(rt);
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
	camera->focal_lenth = ((float)rt->canvas->width * 0.5) / tanf(rad * 0.5);
	reset_camera(camera);
}

void	init_mini_rt(char **argv, t_rt *rt)
{
	load_scene(argv[1], rt);
	init_mlx(argv[1], rt);
	init_camera(&rt->camera, rt);
	init_hooks(rt);
}
