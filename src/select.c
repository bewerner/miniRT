/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 00:38:09 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/14 12:20:04 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

void	unselect_all(t_object *obj)
{
	while (obj)
	{
		obj->is_selected = false;
		obj = obj->next;
	}
}

void	rt_select(t_rt *rt)
{
	t_ivec2		mpos;
	t_ray		ray;
	t_hitpoint	hp;

	if (rt->mode != MODE_SOLID)
		return ;
	mlx_get_mouse_pos(rt->mlx, &mpos.x, &mpos.y);
	ray.origin = rt->camera.origin;
	ray.dir = get_pixel_ray(mpos.x, mpos.y, rt);
	hp = get_closest_hitpoint(ray, rt);
	if (hp.object)
	{
		if (hp.object->is_selected)
		{
			hp.object->is_selected = false;
			return ;
		}
		unselect_all(rt->objects);
		hp.object->is_selected = true;
	}
}


//	color_convert.c
	// uint32_t	rgba(int r, int g, int b, int a)
	// {
	// 	return (0 | (r << 24) | (g << 16) | (b << 8) | a);
	// }

//	hooks.c
	// if (button == MLX_MOUSE_BUTTON_LEFT && action == MLX_PRESS && !modifier)
	// {
	// 	rt_select(rt);
	// 	return ;
	// }

// render.c
	// in trace_ray:
	// if (rt->mode == MODE_SOLID)
	// {		
	// 	if (hitpoint.object->is_selected && is_outline(pixel, hitpoint, rt))
	// 	{
	// 		put_outline_pixel(pixel, rt);
	// 		return ;	
	// 	}
	// 	col = get_solid_color(hitpoint, rt);
	// }

// render.c add those func
	// static bool	is_outline(t_ivec2 px, t_hitpoint hp, t_rt *rt)
	// {
	// 	t_hitpoint	op;
	// 	t_ray		ray;
		
	// 	ray.origin = rt->camera.origin;

	// 	ray.dir = get_pixel_ray(px.x - 1, px.y, rt);
	// 	op = get_closest_hitpoint(ray, rt);
	// 	if (op.object != hp.object)
	// 		return (true);

	// 	ray.dir = get_pixel_ray(px.x + 1, px.y, rt);
	// 	op = get_closest_hitpoint(ray, rt);
	// 	if (op.object != hp.object)
	// 		return (true);

	// 	ray.dir = get_pixel_ray(px.x, px.y - 1, rt);
	// 	op = get_closest_hitpoint(ray, rt);
	// 	if (op.object != hp.object)
	// 		return (true);

	// 	ray.dir = get_pixel_ray(px.x , px.y + 1, rt);
	// 	op = get_closest_hitpoint(ray, rt);
	// 	if (op.object != hp.object)
	// 		return (true);

	// 	return (false);
	// }

	// static void	put_outline_pixel(t_ivec2 px, t_rt *rt)
	// {
	// 	int32_t	x, y;

	// 	if (px.x == 0 || px.y == 0)
	// 		return ;

	// 	x = px.x - 1;
	// 	y = px.y - 1;

	// 	mlx_put_pixel(rt->canvas, px.x, px.y, rgba(255, 159, 45, 255));
	// 	mlx_put_pixel(rt->canvas, px.x + 1, px.y, rgba(255, 159, 45, 255));
	// 	mlx_put_pixel(rt->canvas, px.x, px.y +1, rgba(255, 159, 45, 255));

	// 	if (x > 0)
	// 		mlx_put_pixel(rt->canvas, x, px.y, rgba(255, 159, 45, 255));
	// 	if (y > 0)
	// 		mlx_put_pixel(rt->canvas, px.x, y, rgba(255, 159, 45, 255));
	// }

// loder.c
	// add this to load_scene at the end
	// unselect_all(rt->objects);


// miniRT.h
	// typedef struct 		s_object
	// {
	// 	t_obj_type		type;
	// 	t_object		*next;
	// 	t_vec3			origin;
	// 	t_vec4			base_color;
	// 	bool			is_selected;
	// }					t_object;

	// select.c
	// void			unselect_all(t_object *obj);
	// void			rt_select(t_rt *rt);
