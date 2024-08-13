/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_primitives.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 12:29:18 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/13 18:20:10 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

t_error	parse_sphere(t_sphere *sphere, t_rt *rt)
{
	char			*line;
	static size_t	count;

	count++;
	if (ft_strncmp(rt->line->content, "SP ", 3) == 0 && count > 1)
		return (RT_ERROR_TOO_MANY_SPHERES);
	line = (char *)rt->line->content +2;
	sphere->type = OBJ_SPHERE;
	sphere->next = (void *)sphere + sizeof(t_sphere);
	sphere->origin.x = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	sphere->origin.y = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	sphere->origin.z = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	sphere->radius = validate_range(get_next_value(&line, rt) * 0.5f, (t_vec2){-INFINITY, INFINITY}, rt);
	sphere->base_color.r = validate_range(get_next_value(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	sphere->base_color.g = validate_range(get_next_value(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	sphere->base_color.b = validate_range(get_next_value(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	sphere->base_color.a = 1.0f;
	return (RT_SUCCESS);
}

t_error	parse_plane(t_plane *plane, t_rt *rt)
{
	char			*line;
	static size_t	count;

	count++;
	if (ft_strncmp(rt->line->content, "PL ", 3) == 0  && count > 1)
		return (RT_ERROR_TOO_MANY_PLANES);
	line = (char *)rt->line->content + 2;
	plane->type = OBJ_PLANE;
	plane->next = (void *)plane + sizeof(t_plane);
	plane->origin.x = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	plane->origin.y = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	plane->origin.z = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	plane->normal.x = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	plane->normal.y = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	plane->normal.z = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	plane->normal = vec3_normalize(plane->normal);
	plane->dist = vec3_dot(plane->origin, plane->normal);
	plane->base_color.r = validate_range(get_next_value(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	plane->base_color.g = validate_range(get_next_value(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	plane->base_color.b = validate_range(get_next_value(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	plane->base_color.a = 1.0f;
	return (RT_SUCCESS);
}

t_error	parse_cylinder(t_cylinder *cylinder, t_rt *rt)
{
	char			*line;
	static size_t	count;

	count++;
	if (ft_strncmp(rt->line->content, "CY ", 3) == 0  && count > 1)
		return (RT_ERROR_TOO_MANY_CYLINDER);
	line = (char *)rt->line->content + 2;
	cylinder->type = OBJ_CYLINDER;
	cylinder->next = (void *)cylinder + sizeof(t_cylinder);
	cylinder->origin.x = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	cylinder->origin.y = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	cylinder->origin.z = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	
	cylinder->orientation.x = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	cylinder->orientation.y = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	cylinder->orientation.z = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	cylinder->orientation = vec3_normalize(cylinder->orientation);

	cylinder->radius = validate_range(get_next_value(&line, rt) * 0.5f, (t_vec2){-INFINITY, INFINITY}, rt);
	cylinder->height = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	
	cylinder->base_color.r = validate_range(get_next_value(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	cylinder->base_color.g = validate_range(get_next_value(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	cylinder->base_color.b = validate_range(get_next_value(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	cylinder->base_color.a = 1.0f;

	cylinder->cap1.origin = vec3_sub(cylinder->origin, vec3_scale(cylinder->height / 2, cylinder->orientation));
	cylinder->cap2.origin = vec3_add(cylinder->origin, vec3_scale(cylinder->height / 2, cylinder->orientation));
	cylinder->cap1.normal = vec3_scale(-1, cylinder->orientation);
	cylinder->cap2.normal = cylinder->orientation;
	cylinder->cap1.dist = vec3_dot(cylinder->cap1.origin, cylinder->cap1.normal);
	cylinder->cap2.dist = vec3_dot(cylinder->cap2.origin, cylinder->cap2.normal);
	cylinder->cap1.base_color = cylinder->base_color;
	cylinder->cap2.base_color = cylinder->base_color;

	return (RT_SUCCESS);
}

// printf("l.x -> %f \n", rt->lights[count - 1].origin.x);
// printf("l.y -> %f \n", rt->lights[count - 1].origin.y);
// printf("l.z -> %f \n", rt->lights[count - 1].origin.z);
