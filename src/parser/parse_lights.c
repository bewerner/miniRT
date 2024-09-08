/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lights.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 12:31:54 by nmihaile          #+#    #+#             */
/*   Updated: 2024/09/08 19:14:17 by bwerner          ###   ########.fr       */
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
	pl->power = gnv(&line, rt) * LIGHT_POWER;
	if (fabsf(pl->power) <= 1.0f)
		pl->power *= MAX_POWER;
	pl->intensity = pl->power / 22.6337f;
	pl->color.r = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	pl->color.g = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	pl->color.b = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	pl->color.a = 1.0f;
	return (RT_SUCCESS);
}
