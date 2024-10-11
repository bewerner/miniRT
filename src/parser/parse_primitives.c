/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_primitives.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 12:29:18 by nmihaile          #+#    #+#             */
/*   Updated: 2024/10/11 15:20:24 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

t_error	parse_sphere(t_sphere *sp, t_rt *rt)
{
	char			*line;

	line = (char *)rt->line->content +2;
	sp->type = OBJ_SPHERE;
	sp->next = (void *)sp + sizeof(t_sphere);
	sp->origin.x = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	sp->origin.y = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	sp->origin.z = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	sp->radius = vr(gnv(&line, rt) * 0.5f, (t_vec2){-INFINITY, INFINITY}, rt);
	set_color_and_material(&sp->base_color, &sp->material, line, rt);
	return (RT_SUCCESS);
}

t_error	parse_plane(t_plane *pl, t_rt *rt)
{
	char			*line;

	line = (char *)rt->line->content + 2;
	pl->type = OBJ_PLANE;
	pl->next = (void *)pl + sizeof(t_plane);
	pl->origin.x = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	pl->origin.y = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	pl->origin.z = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	pl->normal.x = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	pl->normal.y = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	pl->normal.z = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	pl->normal = vn(vec3_normalize(pl->normal), rt);
	pl->dist = vec3_dot(pl->origin, pl->normal);
	set_color_and_material(&pl->base_color, &pl->material, line, rt);
	return (RT_SUCCESS);
}

static t_vec3	parse_vec3_infinity(char **line, t_rt *rt)
{
	t_vec3	v;

	v.x = vr(gnv(line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	v.y = vr(gnv(line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	v.z = vr(gnv(line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	return (v);
}

static void	init_caps(t_cylinder *cy)
{
	cy->cap1.type = OBJ_PLANE;
	cy->cap2.type = OBJ_PLANE;
	cy->cap1.origin = vec3_sub(cy->origin,
			vec3_scale(cy->height / 2, cy->orientation));
	cy->cap2.origin = vec3_add(cy->origin,
			vec3_scale(cy->height / 2, cy->orientation));
	cy->cap1.normal = vec3_scale(-1, cy->orientation);
	cy->cap2.normal = cy->orientation;
	cy->cap1.dist = vec3_dot(cy->cap1.origin, cy->cap1.normal);
	cy->cap2.dist = vec3_dot(cy->cap2.origin, cy->cap2.normal);
	cy->cap1.base_color = cy->base_color;
	cy->cap1.material = cy->material;
	cy->cap2.base_color = cy->base_color;
	cy->cap2.material = cy->material;
}

t_error	parse_cylinder(t_cylinder *cy, t_rt *rt)
{
	char			*line;

	line = (char *)rt->line->content + 2;
	cy->type = OBJ_CYLINDER;
	cy->next = (void *)cy + sizeof(t_cylinder);
	cy->origin = parse_vec3_infinity(&line, rt);
	cy->orientation = parse_vec3_infinity(&line, rt);
	cy->orientation = vn(vec3_normalize(cy->orientation), rt);
	cy->radius = vr(gnv(&line, rt) * 0.5f, (t_vec2){-INFINITY, INFINITY}, rt);
	cy->height = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	set_color_and_material(&cy->base_color, &cy->material, line, rt);
	init_caps(cy);
	return (RT_SUCCESS);
}
