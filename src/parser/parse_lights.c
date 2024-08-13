/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lights.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 12:31:54 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/13 19:12:14 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

t_error	parse_light(t_rt *rt)
{
	char			*line;
	static size_t	count;

	count++;
	if (*(char *)(rt->line->content) == 'L' && count > 1)
		return (RT_ERROR_TOO_MANY_LIGHTS);
	line = (char *)rt->line->content + 1;
	rt->lights[count - 1].type = LIGHT_POINT;
	rt->lights[count - 1].origin.x = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	rt->lights[count - 1].origin.y = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	rt->lights[count - 1].origin.z = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	rt->lights[count - 1].ratio = vr(gnv(&line, rt), (t_vec2){0.0f, 1.0f}, rt) * LIGHT_POWER;
	rt->lights[count - 1].power = rt->lights[count - 1].ratio * MAX_POWER;
	rt->lights[count - 1].intensity = rt->lights[count - 1].power / 38.26f;
	rt->lights[count - 1].color.r = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	rt->lights[count - 1].color.g = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	rt->lights[count - 1].color.b = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	rt->lights[count - 1].color.a = 1.0f;
	return (RT_SUCCESS);
}

// printf("l.x -> %f \n", rt->lights[count - 1].origin.x);
// printf("l.y -> %f \n", rt->lights[count - 1].origin.y);
// printf("l.z -> %f \n", rt->lights[count - 1].origin.z);
