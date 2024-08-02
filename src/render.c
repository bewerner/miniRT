/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:56:20 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/02 22:29:29 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

// void	set_directions(t_FdF *s)
// {
// 	t_vec3	vec;

// 	vec = (t_vec3){1, 0, 0};
// 	vec.x = s->mesh_scale * cos(s->yaw);
// 	vec.y = s->mesh_scale * sin(s->yaw);
// 	vec.z = 0;
// 	s->x_dir.x = vec.x;
// 	s->x_dir.y = vec.y * cos(s->pitch) + vec.z * -sin(s->pitch);
// 	s->x_dir.z = vec.y * sin(s->pitch) + vec.z * cos(s->pitch);
// 	vec = (t_vec3){0, 1, 0};
// 	vec.x = s->mesh_scale * -sin(s->yaw);
// 	vec.y = s->mesh_scale * cos(s->yaw);
// 	vec.z = 0;
// 	s->y_dir.x = vec.x;
// 	s->y_dir.y = vec.y * cos(s->pitch) + vec.z * -sin(s->pitch);
// 	s->y_dir.z = vec.y * sin(s->pitch) + vec.z * cos(s->pitch);
// 	vec = (t_vec3){0, 0, 1};
// 	vec.x = 0;
// 	vec.y = 0;
// 	vec.z = s->height_scale * s->mesh_scale;
// 	s->z_dir.x = vec.x;
// 	s->z_dir.y = vec.y * cos(s->pitch) + vec.z * -sin(s->pitch);
// 	s->z_dir.z = vec.y * sin(s->pitch) + vec.z * cos(s->pitch);
// }

// yaw = atan(y/x)
// pitch = atan(z/sqrt(x^2+y^2))

t_vec3	vec3_rotate(t_vec3 v, double pitch, double yaw)
{
	v.x = cos(yaw);
	v.y = sin(yaw);

	return (v);
}

t_vec3	get_pixel_ray(t_camera *camera, uint32_t x, uint32_t y)
{
	t_vec3	unit_z;
	t_vec3	unit_x;
	t_vec3	unit_y;
	double	pitch;
	double	yaw;

	// unit_z = (t_vec3){1, 0, 0};
	unit_z = vec3_normalize(camera->direction);
	unit_x = (t_vec3){0, 1, 0};
	unit_y = (t_vec3){0, 0, 1};
	pitch = atan(unit_z.z / sqrt(unit_z.x * unit_z.x + unit_z.y * unit_z.y)) * -1;
	yaw = atan(unit_z.y / unit_z.x) * -1;
	unit_x = vec3_rotate(unit_x, pitch, yaw);
	unit_y = vec3_rotate(unit_y, pitch, yaw);
}

void	render(t_rt *rt)
{
	uint32_t	x;
	uint32_t	y;
	t_vec3	a = rt->camera.position;
	t_vec3	b;
	t_vec3	f = vec3_scale(rt->camera.focal_lenth, vec3_normalize(rt->camera.direction));
	// f = vec3_add(a, f);
	
	// double	t;
	double	discriminant;
	double	rr;
	uint32_t	half_w = rt->canvas->width / 2;
	uint32_t	half_h = rt->canvas->height / 2;

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

			b = get_pixel_ray(&rt->camera, x, y);

			double	A = vec3_dot(b, b);
			double	B = 2 * vec3_dot(AP, b);
			// double	B = 2 * vec3_dot(AP, b);
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
