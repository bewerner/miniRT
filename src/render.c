/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:56:20 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/03 21:21:54 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

t_vec3	get_pixel_ray(uint32_t x, uint32_t y, t_rt *rt)
{
	t_vec3	v;

	v = vec3_add(rt->screen.pos_null, vec3_scale(x, rt->screen.x_dir));
	v = vec3_add(v, vec3_scale(y, rt->screen.y_dir));
	return (v);
}

t_hitpoint	get_hitpoint_sphere(t_vec3 rayDir, t_object *object, t_rt *rt)
{
	t_hitpoint	hitpoint;
	double		discriminant;
	double		rr;
	t_vec3		AP;

	hitpoint.object = object;

	rr = object->radius * object->radius;
	AP = vec3_sub(rt->camera.origin, object->origin);

	double	A = vec3_dot(rayDir, rayDir);
	double	B = 2 * vec3_dot(AP, rayDir);
	double	C = vec3_dot(AP, AP) - rr;

	discriminant = B * B - 4 * A * C;

	if (discriminant < 0)
		return ((t_hitpoint){VEC3_INF, NULL});

	// double	t0 = (-B  sqrt(discriminant)) / (2 * A)		// DO WE NEED THIS LATER ?? 
	double	t1 = (-B - sqrt(discriminant)) / (2 * A);

	if (t1 < 0)
		return ((t_hitpoint){VEC3_INF, NULL});

	// HitPoint
	hitpoint.pos = vec3_add(rt->camera.origin, vec3_scale(t1, rayDir));

	return (hitpoint);
}

t_hitpoint	get_closest_hitpoint(t_vec3 rayDir, t_rt *rt)
{
	size_t		i;
	t_hitpoint	closest;
	t_hitpoint	current;

	closest.pos = VEC3_INF;
	closest.object = NULL;
	i = 0;
	// while (rt->objects[i])
	while (rt->objects[i].type)
	{
		if (rt->objects[i].type == OBJ_SPHERE)
			current = get_hitpoint_sphere(rayDir, &rt->objects[i], rt);
		// else if (rt->object[i].type == OBJ_SPHERE)
		// else if (rt->object[i].type == OBJ_SPHERE)
		if (vec3_len(current.pos) < vec3_len(closest.pos))
			closest = current;
		i++;
	}
	return (closest);
}


t_vec4	get_diffuse_color_sphere(t_hitpoint hitpoint, t_rt *rt)
{
	t_vec4	col;
	t_vec3	normal;
	t_vec3	hitpoint_light;
	
	normal = vec3_normalize(vec3_sub(hitpoint.object->origin, hitpoint.pos));
	hitpoint_light = vec3_normalize(vec3_sub(hitpoint.pos, rt->lights[0].origin));
	double	light = vec3_dot(normal, hitpoint_light);
	// col = (t_vec4){{normal.x * 0.5f + 0.5f, normal.y * 0.5f + 0.5f, normal.z * 0.5f + 0.5f, 1.0f}};

	if (light < 0)
		col = VEC4_BLACK;
	else
		col = (t_vec4){{light, 0, 0, 1.0f}};

	col = vec4_add(col, (t_vec4){{.025, .025, .1, 1}});

	return (col);
}

t_vec4	get_diffuse_color(t_hitpoint hitpoint, t_rt *rt)
{
	if (hitpoint.object->type == OBJ_SPHERE)
		return (get_diffuse_color_sphere(hitpoint, rt));
	// else if (rt->object[i].type == OBJ_SPHERE)
	// else if (rt->object[i].type == OBJ_SPHERE)
	return (VEC4_BLACK);
}

void	trace_ray(t_ivec2 pixel, t_rt *rt)
{
	t_vec3		rayDir;
	t_hitpoint	hitpoint;
	t_vec4		col;

	rayDir = get_pixel_ray(pixel.x, pixel.y, rt);
	// get closest hitpoint (and object pointer)
	hitpoint = get_closest_hitpoint(rayDir, rt);
	if (!hitpoint.object)
		return ;
	col = get_diffuse_color(hitpoint, rt);
	mlx_put_pixel(rt->canvas, pixel.x, pixel.y, vec4_to_rgba(col));
	// putpixel color
}

void	render(t_rt *rt)
{
	t_ivec2	pixel;

	pixel.y = 0;
	while (pixel.y < (int32_t)rt->canvas->height)
	{
		pixel.x = 0;
		while (pixel.x < (int32_t)rt->canvas->width)
		{
			trace_ray(pixel, rt);
			pixel.x++;
		}
		pixel.y++;
	}
}
