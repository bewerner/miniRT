/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_primitives.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 12:29:18 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/09 12:42:03 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

t_error	parse_sphere(size_t *obj_count, t_rt *rt)
{
	char			*line;
	static size_t	count;

	count++;
	if (*(char *)(rt->line->content) + *(char *)(rt->line->content + 1) == 163 && count > 1)
		return (RT_ERROR_TOO_MANY_SPHERES);
	line = &rt->line->content[2];
	rt->objects[*obj_count].type = OBJ_SPHERE;
	rt->objects[*obj_count].origin.x = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	rt->objects[*obj_count].origin.y = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	rt->objects[*obj_count].origin.z = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	rt->objects[*obj_count].radius = validate_range(get_next_value(&line, rt) * 0.5f, (t_vec2){-INFINITY, INFINITY}, rt);
	rt->objects[*obj_count].base_color.r = validate_range(get_next_value(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	rt->objects[*obj_count].base_color.g = validate_range(get_next_value(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	rt->objects[*obj_count].base_color.b = validate_range(get_next_value(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	rt->objects[*obj_count].base_color.a = 1.0f;
	(*obj_count)++;
	return (RT_SUCCESS);
}

t_error	parse_plane(size_t *obj_count, t_rt *rt)
{
	char			*line;
	static size_t	count;

	count++;
	line = &rt->line->content[2];
	rt->objects[*obj_count].type = OBJ_PLANE;
	rt->objects[*obj_count].origin.x = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	rt->objects[*obj_count].origin.y = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	rt->objects[*obj_count].origin.z = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	rt->objects[*obj_count].normal.x = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	rt->objects[*obj_count].normal.y = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	rt->objects[*obj_count].normal.z = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	rt->objects[*obj_count].normal = vec3_normalize(rt->objects[*obj_count].normal);
	rt->objects[*obj_count].dist = vec3_dot(rt->objects[*obj_count].origin, rt->objects[*obj_count].normal);
	rt->objects[*obj_count].base_color.r = validate_range(get_next_value(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	rt->objects[*obj_count].base_color.g = validate_range(get_next_value(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	rt->objects[*obj_count].base_color.b = validate_range(get_next_value(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	rt->objects[*obj_count].base_color.a = 1.0f;
	(*obj_count)++;
	return (RT_SUCCESS);
}

// printf("l.x -> %f \n", rt->lights[count - 1].origin.x);
// printf("l.y -> %f \n", rt->lights[count - 1].origin.y);
// printf("l.z -> %f \n", rt->lights[count - 1].origin.z);
