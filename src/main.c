/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 15:10:10 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/10 21:08:35 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

void	init_mlx(t_rt *rt)
{
	// int32_t	width;
	// int32_t	height;
	// float	ratio;

	rt->mlx = mlx_init(WINDOW_WIDTH, WINDOW_HEIGHT, "miniRT", 1);
	if (!rt->mlx)
		terminate("mlx_init failed", 1, rt);
	// mlx_get_monitor_size(0, &width, &height);
	// ratio = (float)WINDOW_WIDTH / WINDOW_HEIGHT;
	// if (rt->mlx->width > width)
	// {
	// 	height = width / ratio;
	// 	mlx_set_window_size(rt->mlx, width, height);
	// 	printf("%d x %d\n", rt->mlx->width, rt->mlx->height);
	// }
	// if (height < WINDOW_HEIGHT)
	// {
	// 	width = height * ratio;
	// 	mlx_set_window_size(rt->mlx, width, height);
	// 	printf("%d x %d\n", rt->mlx->width, rt->mlx->height);
	// }
	// mlx_get_monitor_size(0, &width, &height);
	// mlx_set_window_pos(rt->mlx,
	// 	((width - rt->mlx->width) / 2), ((height - rt->mlx->height) / 2));
	rt->canvas = mlx_new_image(rt->mlx, rt->mlx->width, rt->mlx->height);
	if (!rt->canvas || mlx_image_to_window(rt->mlx, rt->canvas, 0, 0) < 0)
		terminate("mlx_new_image failed", 1, rt);
}

void	validate_args(int argc, char **argv, t_rt *rt)
{
	if (argc != 2)
		terminate("usage: ./miniRt scene_file.rt", 1, rt);
	if (ft_strlen(argv[1]) <= 3
		|| !ft_strrchr(argv[1], '.')
		|| ft_strncasecmp(ft_strrchr(argv[1], '.'), ".rt\0", 4))
	{
		terminate(error_msg(RT_ERROR_WRONG_FILE_FORMAT), 1, rt);
	}
}

void	init_camera(t_camera* camera, t_rt *rt)
{
	t_vec3	dir;

	camera->direction = vec3_normalize(camera->direction);
	dir = camera->direction;
	// get camera.pitch and camera.yaw from camera.direction
	camera->pitch = atan2(sqrt(dir.x * dir.x + dir.y * dir.y), -dir.z);
	camera->yaw = atan2(dir.x, dir.y) * -1;
	// printf("yaw:   %10f = %4.1f\npitch: %10f = %4.1f\n", camera->yaw, camera->yaw * 180 / M_PI, camera->pitch, camera->pitch * 180 / M_PI);

	float rad = camera->fov * (M_PI / 180);

	camera->focal_lenth = ((float)rt->canvas->width * 0.5) / tan(rad * 0.5 );
	// printf("focal length -> %f \n", camera->focal_lenth);

}

int	main(int argc, char **argv)
{
	t_rt	rt;

	errno = 0;
	ft_bzero(&rt, sizeof(t_rt));
	validate_args(argc, argv, &rt);
	load_scene(argv[1], &rt);
	init_mlx(&rt);
	init_camera(&rt.camera, &rt);
	init_hooks(&rt);
	mlx_loop(rt.mlx);
	terminate(NULL, 0, &rt);
	return (0);
}
