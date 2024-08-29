/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resize_hook.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 19:46:24 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/29 21:23:02 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

void	resize_hook(GLFWwindow *window, int width, int height)
{
	t_rt	*rt;
	float	rad;

	rt = get_rt();
	rad = rt->camera.fov * (M_PI / 180);
	rt->camera.focal_length = 1.0f / tan(rad * 0.5);
	glViewport(0, 0, width, height);
	glfwGetWindowSize(window, &rt->width, &rt->height);
}

// void	resize_hook(int width, int height, void *param)
// {
// 	t_rt	*rt;
// 	float	rad;

// 	rt = (t_rt *)param;
// 	rad = rt->camera.fov * (M_PI / 180);
// 	rt->camera.focal_lenth = ((float)width * 0.5) / tanf(rad * 0.5);
// 	mlx_resize_image(rt->canvas, width, height);
// }
