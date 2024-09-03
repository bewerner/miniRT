/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_primitives.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 12:29:18 by nmihaile          #+#    #+#             */
/*   Updated: 2024/09/03 20:43:01 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static t_material	*get_next_material(char *line, t_rt *rt)
{
	t_material	*material;

	ft_skipspace(&line);
	ft_terminate_after_word(line);
	if (*line == '\0')
		return (rt->materials);
	material = rt->materials;
	while (material && ft_strcmp(material->name, line) != 0)
		material = material->next;
	if (material == NULL)
		terminate("invalid material name detected", 1, rt);		// terminate if we find an ivalid material name
		// return (rt->materials);								// invalid material name defaults to default material
	return (material);
}

static void	set_color_and_material(t_vec4 *col, t_material **mat, char *line, t_rt *rt)
{
	ft_skipspace(&line);
	if (ft_isalpha(*line))
	{
		printf("No COLOR -> MAT only\n");
		*mat = get_next_material(line, rt);
		*col = (t_vec4){{(*mat)->color.r, (*mat)->color.g, (*mat)->color.b, 1.0f}};
	}
	else
	{
		col->r = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
		col->g = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
		col->b = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
		col->a = 1.0f;
		// *mat = rt->materials;
		*mat = get_next_material(line, rt);
	}
}

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
	// sp->base_color.r = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	// sp->base_color.g = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	// sp->base_color.b = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	// sp->base_color.a = 1.0f;
	// sp->material = get_next_material(line, rt);
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
	pl->normal = vec3_normalize(pl->normal);
	pl->dist = vec3_dot(pl->origin, pl->normal);
	// pl->base_color.r = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	// pl->base_color.g = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	// pl->base_color.b = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	// pl->base_color.a = 1.0f;
	// pl->material = get_next_material(line, rt);
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
	cy->orientation = vec3_normalize(cy->orientation);
	cy->radius = vr(gnv(&line, rt) * 0.5f, (t_vec2){-INFINITY, INFINITY}, rt);
	cy->height = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	// cy->base_color.r = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	// cy->base_color.g = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	// cy->base_color.b = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	// cy->base_color.a = 1.0f;
	// cy->material = get_next_material(line, rt);
	set_color_and_material(&cy->base_color, &cy->material, line, rt);
	init_caps(cy);
	return (RT_SUCCESS);
}
