/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 15:10:10 by nmihaile          #+#    #+#             */
/*   Updated: 2024/07/31 16:47:15 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

void	init_mlx(t_rt *rt)
{
	// int32_t	width;
	// int32_t	height;
	// double	ratio;

	rt->mlx = mlx_init(WINDOW_WIDTH, WINDOW_HEIGHT, "miniRT", 1);
	if (!rt->mlx)
		terminate("mlx_init failed", 1, rt);
	// mlx_get_monitor_size(0, &width, &height);
	// ratio = (double)WINDOW_WIDTH / WINDOW_HEIGHT;
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

void	validate_input(int argc, char **argv, t_rt *rt)
{
	if (argc != 2)
		terminate("usage: ./miniRt scene_file.rt", 1, rt);
	if (ft_strlen(argv[1]) <= 3
		|| !ft_strrchr(argv[1], '.')
		|| ft_strncasecmp(ft_strrchr(argv[1], '.'), ".rt\0", 4))
	{
		terminate("error: wrong file format. (needs to be .rt)", 1, rt);
	}
}

void	parse_scene(char *file, t_rt *rt)
{
	(void)file;
	rt->objects = (t_object *)ft_calloc(1, sizeof(t_object));
	rt->objects[0].type = OBJ_SPHERE;
	rt->objects[0].origin = (t_vec3){5, 0, 0};
	rt->objects[0].radius = 1;
	rt->camera.position = (t_vec3){-5, 0, 0};
	rt->camera.direction = (t_vec3){1, 0, 0};
	rt->camera.focal_lenth = 1020;
}

int	main(int argc, char **argv)
{
	t_rt	rt;

	errno = 0;
	ft_bzero(&rt, sizeof(t_rt));
	validate_input(argc, argv, &rt);
	parse_scene(argv[1], &rt);
	init_mlx(&rt);
	init_hooks(&rt);
	mlx_loop(rt.mlx);
	terminate(NULL, 0, &rt);
	return (0);
}
