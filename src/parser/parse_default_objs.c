/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_default_objs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 12:30:47 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/13 19:22:10 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

t_error	parse_ambient(t_rt *rt)
{
	float			ratio;
	char			*line;
	static size_t	count;

	count++;
	if (count > 1)
		return (RT_ERROR_TOO_MANY_AMBIENT);
	if (ft_strncmp(rt->line->content, "a ", 2) == 0)
		return (RT_ERROR_AMBIENT_LOWER_CASE);
	line = (char *)rt->line->content + 1;
	ratio = vr(gnv(&line, rt), (t_vec2){0.0f, 1.0f}, rt);
	rt->ambient.r = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	rt->ambient.g = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	rt->ambient.b = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	rt->ambient = vec4_scale(ratio, rt->ambient);
	rt->ambient.a = 1.0f;
	return (RT_SUCCESS);
}

t_error	parse_camera(t_rt *rt)
{
	char			*line;
	static size_t	count;
	t_camera		*cam;

	count++;
	if (count > 1)
		return (RT_ERROR_TOO_MANY_CAMERAS);
	else if (ft_strncmp(rt->line->content, "c ", 2) == 0)
		return (RT_ERROR_CAMERA_LOWER_CASE);
	line = (char *)rt->line->content + 1;
	cam = &rt->camera;
	cam->origin.x = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	cam->origin.y = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	cam->origin.z = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	cam->direction.x = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	cam->direction.y = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	cam->direction.z = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	cam->direction = vec3_normalize(cam->direction);
	cam->fov = vr(gnv(&line, rt), (t_vec2){0.0f, 180.0f}, rt);
	return (RT_SUCCESS);
}

// printf("l.x -> %f \n", rt->lights[count - 1].origin.x);
// printf("l.y -> %f \n", rt->lights[count - 1].origin.y);
// printf("l.z -> %f \n", rt->lights[count - 1].origin.z);
