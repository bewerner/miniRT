/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lights.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 12:31:54 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/13 20:26:19 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

t_error	parse_point_light(t_point_light *pl, t_rt *rt)
{
	char			*line;
	static size_t	count;

	count++;
	if (*(char *)(rt->line->content) == 'L' && count > 1)
		return (RT_ERROR_TOO_MANY_LIGHTS);
	line = (char *)rt->line->content + 1;
	pl->type = LIGHT_POINT;
	pl->next = (void *)pl + sizeof(t_point_light);
	pl->origin.x = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	pl->origin.y = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	pl->origin.z = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	pl->ratio = vr(gnv(&line, rt), (t_vec2){0.0f, 1.0f}, rt) * LIGHT_POWER;
	pl->power = pl->ratio * MAX_POWER;
	pl->intensity = pl->power / 38.26f;
	pl->color.r = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	pl->color.g = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	pl->color.b = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	pl->color.a = 1.0f;
	return (RT_SUCCESS);
}

// printf("l.x -> %f \n", pl->origin.x);
// printf("l.y -> %f \n", pl->origin.y);
// printf("l.z -> %f \n", pl->origin.z);
