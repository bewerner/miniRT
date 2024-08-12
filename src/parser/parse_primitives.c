/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_primitives.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 12:29:18 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/12 22:06:36 by nmihaile         ###   ########.fr       */
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

	// rt->objects[*obj_count].type = OBJ_SPHERE;
	// rt->objects[*obj_count].origin.x = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	// rt->objects[*obj_count].origin.y = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	// rt->objects[*obj_count].origin.z = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	// rt->objects[*obj_count].radius = validate_range(get_next_value(&line, rt) * 0.5f, (t_vec2){-INFINITY, INFINITY}, rt);
	// rt->objects[*obj_count].base_color.r = validate_range(get_next_value(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	// rt->objects[*obj_count].base_color.g = validate_range(get_next_value(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	// rt->objects[*obj_count].base_color.b = validate_range(get_next_value(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	// rt->objects[*obj_count].base_color.a = 1.0f;
	// (*obj_count)++;
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


	// rt->objects[*obj_count].type = OBJ_PLANE;
	// rt->objects[*obj_count].origin.x = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	// rt->objects[*obj_count].origin.y = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	// rt->objects[*obj_count].origin.z = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	// rt->objects[*obj_count].normal.x = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	// rt->objects[*obj_count].normal.y = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	// rt->objects[*obj_count].normal.z = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	// rt->objects[*obj_count].normal = vec3_normalize(rt->objects[*obj_count].normal);
	// rt->objects[*obj_count].dist = vec3_dot(rt->objects[*obj_count].origin, rt->objects[*obj_count].normal);
	// rt->objects[*obj_count].base_color.r = validate_range(get_next_value(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	// rt->objects[*obj_count].base_color.g = validate_range(get_next_value(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	// rt->objects[*obj_count].base_color.b = validate_range(get_next_value(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	// rt->objects[*obj_count].base_color.a = 1.0f;
	// (*obj_count)++;
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
	
	cylinder->cap1 = vec3_sub(cylinder->origin, vec3_scale(cylinder->height / 2, cylinder->orientation));
	cylinder->cap2 = vec3_add(cylinder->origin, vec3_scale(cylinder->height / 2, cylinder->orientation));
	
	cylinder->base_color.r = validate_range(get_next_value(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	cylinder->base_color.g = validate_range(get_next_value(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	cylinder->base_color.b = validate_range(get_next_value(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	cylinder->base_color.a = 1.0f;

	// rt->objects[*obj_count].type = OBJ_CYLINDER;
	// rt->objects[*obj_count].origin.x = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	// rt->objects[*obj_count].origin.y = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	// rt->objects[*obj_count].origin.z = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	
	// rt->objects[*obj_count].orientation.x = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	// rt->objects[*obj_count].orientation.y = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	// rt->objects[*obj_count].orientation.z = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	// rt->objects[*obj_count].orientation = vec3_normalize(rt->objects[*obj_count].orientation);

	// rt->objects[*obj_count].radius = validate_range(get_next_value(&line, rt) * 0.5f, (t_vec2){-INFINITY, INFINITY}, rt);
	// rt->objects[*obj_count].height = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	
	// rt->objects[*obj_count].cap1 = vec3_sub(rt->objects[*obj_count].origin, vec3_scale(rt->objects[*obj_count].height / 2, rt->objects[*obj_count].orientation));
	// rt->objects[*obj_count].cap2 = vec3_add(rt->objects[*obj_count].origin, vec3_scale(rt->objects[*obj_count].height / 2, rt->objects[*obj_count].orientation));
	
	// rt->objects[*obj_count].base_color.r = validate_range(get_next_value(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	// rt->objects[*obj_count].base_color.g = validate_range(get_next_value(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	// rt->objects[*obj_count].base_color.b = validate_range(get_next_value(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	// rt->objects[*obj_count].base_color.a = 1.0f;
	// (*obj_count)++;
	return (RT_SUCCESS);
}

// printf("l.x -> %f \n", rt->lights[count - 1].origin.x);
// printf("l.y -> %f \n", rt->lights[count - 1].origin.y);
// printf("l.z -> %f \n", rt->lights[count - 1].origin.z);
