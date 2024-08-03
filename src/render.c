/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:56:20 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/03 17:03:39 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

t_vec3	get_pixel_ray(uint32_t x, uint32_t y, t_rt *rt)
{
	t_vec3	v;

	v = vec3_add(rt->screen.pos_null, vec3_scale(x, rt->screen.x));
	v = vec3_add(v, vec3_scale(y, rt->screen.y));
	return (v);
}

t_vec4	per_pixel(t_ivec2 pixel, t_vec3 AP, double rr, t_rt *rt)
{
	double	discriminant;
	t_vec3	rayDir;
	t_vec4	col;

	rayDir = get_pixel_ray(pixel.x, pixel.y, rt);

	double	A = vec3_dot(rayDir, rayDir);
	double	B = 2 * vec3_dot(AP, rayDir);
	double	C = vec3_dot(AP, AP) - rr;

	discriminant = B * B - 4 * A * C;

	if (discriminant < 0)
		return (VEC4_BLACK);

	// double	t0 = (-B  sqrt(discriminant)) / (2 * A)		// DO WE NEED THIS LATER ?? 
	double	t1 = (-B - sqrt(discriminant)) / (2 * A);

	// HitPoint
	t_vec3	hitpoint = vec3_add(rt->camera.origin, vec3_scale(t1, rayDir));

	// NORMALS
	t_vec3	nor = vec3_normalize(vec3_sub(rt->objects[0].origin, hitpoint));

	t_vec3	hpl = vec3_normalize(vec3_sub(hitpoint, rt->lights[0].origin));

	double	light = vec3_dot(nor, hpl);

	// col = (t_vec4){{nor.x * 0.5f + 0.5f, nor.y * 0.5f + 0.5f, nor.z * 0.5f + 0.5f, 1.0f}};
	if (light < 0)
		col = VEC4_BLACK;
	else
		col = (t_vec4){{light, 0, 0, 1.0f}};

	col = vec4_add(col, (t_vec4){{.025, .025, .1, 1}});

	return (col);

}

void	render(t_rt *rt)
{
	t_ivec2	pixel;
	t_vec3	a = rt->camera.origin;
	double	rr;
	t_vec4	col;

	rr = rt->objects[0].radius * rt->objects[0].radius;
	t_vec3	AP = vec3_sub(a, rt->objects[0].origin);
	
	pixel.y = 0;
	while (pixel.y < (int32_t)rt->canvas->height)
	{
		pixel.x = 0;
		while (pixel.x < (int32_t)rt->canvas->width)
		{
			col = per_pixel(pixel, AP, rr, rt);
			mlx_put_pixel(rt->canvas, pixel.x, pixel.y, vec4_to_rgba(col));
			pixel.x++;
		}
		pixel.y++;
	}
}
