/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_default_objs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 12:30:47 by nmihaile          #+#    #+#             */
/*   Updated: 2024/10/23 19:54:30 by bwerner          ###   ########.fr       */
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
	rt->ambient_strength
		= vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	if (next_is_name(line))
	{
		gnv_name(rt->ambient_env_file, &line, rt);
		rt->ambient = (t_vec3){{-1.0f, -1.0f, -1.0f}};
	}
	else
	{
		rt->ambient.r = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
		rt->ambient.g = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
		rt->ambient.b = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
		rt->ambient = vec3_scale(rt->ambient_strength, rt->ambient);
	}
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

t_error	parse_render_settings(t_rt *rt)
{
	char			*line;
	static size_t	count;

	count++;
	if (count > 1)
		return (RT_ERROR_TOO_MANY_RENDER_SETTINGS);
	line = (char *)rt->line->content + 1;
	rt->max_diffuse_bounces = (int)vr(gnv(&line, rt), (t_vec2){0, 10}, rt);
	rt->max_glossy_bounces = (int)vr(gnv(&line, rt), (t_vec2){0, 100}, rt);
	if (has_next_value(line))
		rt->max_samples = (int)vr(gnv(&line, rt), (t_vec2){1, 2147483646}, rt);
	return (RT_SUCCESS);
}
