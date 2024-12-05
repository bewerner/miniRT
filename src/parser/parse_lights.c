/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lights.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 12:31:54 by nmihaile          #+#    #+#             */
/*   Updated: 2024/12/05 16:33:56 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

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
	pl->power = gnv(&line, rt) * 0.575f;
	pl->intensity = pl->power / 22.6337f;
	pl->color.r = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	pl->color.g = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	pl->color.b = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	pl->radius = 0.0f;
	if (has_next_value(line))
		pl->radius = vr(gnv(&line, rt), (t_vec2){0.0f, INFINITY}, rt);
	return (RT_SUCCESS);
}
