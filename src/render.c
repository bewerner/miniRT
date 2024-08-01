/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:56:20 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/01 02:55:13 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

uint32_t	get_color(int r, int g, int b, int a)
{
	return (0 | (r << 24) | (g << 16) | (b << 8) | a);
}

t_vec3	get_pixel_ray(uint32_t x, uint32_t y, t_rt *rt)
{
	t_vec3	unit_x;
	t_vec3	unit_y;
	double	half_w = rt->canvas->width / 2;
	double	half_h = rt->canvas->height / 2;
	t_vec3	screen_origin;
	t_vec3	v;

	unit_x = (t_vec3){0, -1, 0};
	unit_y = (t_vec3){0, 0, -1};
	unit_x = vec3_rotate_z(unit_x, rt->camera.yaw);
	unit_y = vec3_rotate_z(unit_y, rt->camera.yaw);
	unit_y = vec3_rotate_y(unit_y, rt->camera.pitch);
	// printf("yaw: %f = %f\npitch: %f = %f\n", yaw, yaw * 180 / 3.14159, pitch, pitch * 180 / 3.14159);
	// printf("Z: %f, %f, %f\n", unit_z.x, unit_z.y, unit_z.z);
	// printf("X: %f, %f, %f\n", unit_x.x, unit_x.y, unit_x.z);
	// printf("Y: %f, %f, %f\n", unit_y.x, unit_y.y, unit_y.z);
	// printf("\n");
	screen_origin = vec3_scale(rt->camera.focal_lenth, rt->camera.direction);
	v = screen_origin;
	v = vec3_sub(v, vec3_scale(half_w, unit_x));
	v = vec3_sub(v, vec3_scale(half_h, unit_y));
	v = vec3_add(v, vec3_scale(x, unit_x));
	v = vec3_add(v, vec3_scale(y, unit_y));
	return (v);
}

void	render(t_rt *rt)
{
	uint32_t	x;
	uint32_t	y;
	t_vec3	a = rt->camera.position;
	t_vec3	b;
	// t_vec3	f = vec3_scale(rt->camera.focal_lenth, vec3_normalize(rt->camera.direction));
	// f = vec3_add(a, f);
	
	// double	t;
	double	discriminant;
	double	rr;
	// uint32_t	half_w = rt->canvas->width / 2;
	// uint32_t	half_h = rt->canvas->height / 2;

	y = 0;
	rr = rt->objects[0].radius * rt->objects[0].radius;
	t_vec3	AP = vec3_sub(a, rt->objects[0].origin);
	while (y < rt->canvas->height)
	{
		x = 0;
		while (x < rt->canvas->width)
		{
			// b.x = f.x;
			// b.y = f.y + half_w - x;
			// b.z = f.z + half_h - y;

			b = get_pixel_ray(x, y, rt);
			// printf("%f, %f, %f\n", b.x, b.y, b.z);

			double	A = vec3_dot(b, b);
			double	B = 2 * vec3_dot(AP, b);
			double	C = vec3_dot(AP, AP) - rr;

			discriminant = B * B - 4 * A * C;
			// printf("%f\n", discriminant);

			if (discriminant >= 0)
				mlx_put_pixel(rt->canvas, x++, y, get_color(255, 0, 0, 255));
			else
				mlx_put_pixel(rt->canvas, x++, y, get_color(0, 0, 0, 255));
		}
		y++;
	}
}
