/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 15:10:10 by nmihaile          #+#    #+#             */
/*   Updated: 2024/07/30 17:31:07 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

void	init_mlx(t_rt *rt)
{
	int32_t	width;
	int32_t	height;

	rt->mlx = mlx_init(WINDOW_WIDTH, WINDOW_HEIGHT, "miniRT", 1);
	if (!rt->mlx)
		terminate("mlx_init failed", 1, rt);
	mlx_get_monitor_size(0, &width, &height);
	if (width < WINDOW_WIDTH || height < WINDOW_HEIGHT)
	{
		width = ft_imin(width, height);
		height = width;
		mlx_set_window_size(rt->mlx, width, height);
	}
	rt->canvas = mlx_new_image(rt->mlx, rt->mlx->width, rt->mlx->height);
	if (!rt->canvas || mlx_image_to_window(rt->mlx, rt->canvas, 0, 0) < 0)
		terminate("mlx_new_image failed", 1, rt);
}

int	main(int argc, char **argv)
{
	t_rt	rt;

	errno = 0;
	ft_bzero(&rt, sizeof(t_rt));
	(void)argc;
	(void)argv;
	// validate input
	// parse scene
	// open window
	init_mlx(&rt);
	init_hooks(&rt);
	mlx_loop(rt.mlx);
	// render scene
	// wait for close/esc -> terminate

	terminate(NULL, 0, &rt);
	return (0);
}
