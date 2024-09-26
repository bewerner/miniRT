/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_default_objs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 12:30:47 by nmihaile          #+#    #+#             */
/*   Updated: 2024/09/26 15:39:10 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

t_error	parse_ambient(t_rt *rt)
{
	char			*line;
	static size_t	count;

	count++;
	if (count > 1)
		return (RT_ERROR_TOO_MANY_AMBIENT);
	line = (char *)rt->line->content + 1;
	rt->ambient_strength = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	rt->ambient.r = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	rt->ambient.g = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	rt->ambient.b = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	rt->ambient = vec3_scale(rt->ambient_strength, rt->ambient);
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
