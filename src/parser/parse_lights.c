/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lights.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 12:31:54 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/13 11:17:16 by nmihaile         ###   ########.fr       */
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
	rt->lights[count - 1].origin.x = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	rt->lights[count - 1].origin.y = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	rt->lights[count - 1].origin.z = validate_range(get_next_value(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	rt->lights[count - 1].ratio = validate_range(get_next_value(&line, rt), (t_vec2){0.0f, 1.0f}, rt) * LIGHT_POWER;
	rt->lights[count - 1].power = rt->lights[count - 1].ratio * MAX_POWER;
	rt->lights[count - 1].intensity = rt->lights[count - 1].power / 38.26f;
	rt->lights[count - 1].color.r = validate_range(get_next_value(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	rt->lights[count - 1].color.g = validate_range(get_next_value(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	rt->lights[count - 1].color.b = validate_range(get_next_value(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	rt->lights[count - 1].color.a = 1.0f;
	return (RT_SUCCESS);
}

// printf("l.x -> %f \n", rt->lights[count - 1].origin.x);
// printf("l.y -> %f \n", rt->lights[count - 1].origin.y);
// printf("l.z -> %f \n", rt->lights[count - 1].origin.z);
