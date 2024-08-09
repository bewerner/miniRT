/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_default_objs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 12:30:47 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/09 12:35:59 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

t_error	parse_ambient(t_rt *rt)
{
	double			r;
	char			*line;
	static size_t	count;

	count++;
	if (count > 1)
		return (RT_ERROR_TOO_MANY_AMBIENT);
	if (*(char *)(rt->line->content) == 'a')
		return (RT_ERROR_AMBIENT_LOWER_CASE);
	line = &rt->line->content[1];
	r = validate_range(get_next_value(&line, rt), (t_vec2){0.0f, 1.0f}, rt);
	rt->ambient.r = validate_range(get_next_value(&line, rt) / 255.0f, (t_vec2){0.0f, 255.0f}, rt);
	rt->ambient.g = validate_range(get_next_value(&line, rt) / 255.0f, (t_vec2){0.0f, 255.0f}, rt);
	rt->ambient.b = validate_range(get_next_value(&line, rt) / 255.0f, (t_vec2){0.0f, 255.0f}, rt);
	rt->ambient.a = 1.0;
	rt->ambient = vec4_scale(r, rt->ambient);
	return (RT_SUCCESS);
}

t_error	parse_camera(t_rt *rt)
{
	char			*line;
	static size_t	count;

	count++;
	if (count > 1)
		return (RT_ERROR_TOO_MANY_CAMERAS);
	else if (*(char *)(rt->line->content) == 'c')
		return (RT_ERROR_CAMERA_LOWER_CASE);
	line = &rt->line->content[1];
	rt->camera.origin.x = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	rt->camera.origin.y = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	rt->camera.origin.z = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	rt->camera.direction.x = validate_range(get_next_value(&line, rt), (t_vec2){-2.0f, 2.0f}, rt);
	rt->camera.direction.y = validate_range(get_next_value(&line, rt), (t_vec2){-2.0f, 2.0f}, rt);
	rt->camera.direction.z = validate_range(get_next_value(&line, rt), (t_vec2){-2.0f, 2.0f}, rt);
	rt->camera.fov = validate_range(get_next_value(&line, rt), (t_vec2){0.0f, 180.0f}, rt);
	return (RT_SUCCESS);
}

// printf("l.x -> %f \n", rt->lights[count - 1].origin.x);
// printf("l.y -> %f \n", rt->lights[count - 1].origin.y);
// printf("l.z -> %f \n", rt->lights[count - 1].origin.z);
