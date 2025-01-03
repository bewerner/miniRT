/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 18:39:29 by nmihaile          #+#    #+#             */
/*   Updated: 2024/10/31 07:18:59 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

size_t	obj_size(t_identifier id)
{
	if (id == ID_SPHERE)
		return (sizeof(t_sphere));
	else if (id == ID_PLANE)
		return (sizeof(t_plane));
	else if (id == ID_CYLINDER)
		return (sizeof(t_cylinder));
	else if (id == ID_HYPERBOLOID)
		return (sizeof(t_hyperboloid));
	return (0);
}

size_t	light_size(t_identifier id)
{
	if (id == ID_POINT_LIGHT)
		return (sizeof(t_point_light));
	return (0);
}

void	ft_delete_line(char **str)
{
	if (*str)
	{
		ft_free((void *)str);
		*str = NULL;
	}
}

bool	has_next_value(char *line)
{
	while (line && *line && ft_isspace(*line))
		line++;
	if (line && *line && !ft_isspace(*line))
		return (true);
	return (false);
}

// validate normal vector length
// parse_utils3.c
t_vec3	vn(t_vec3 normal, t_rt *rt)
{
	float	len;

	len = vec3_len(normal);
	if (len >= 1.0f - EPSILON && len <= 1.0f + EPSILON)
		return (normal);
	terminate("normal is not a unit vector", rt->curr_line, 1, rt);
	return ((t_vec3){{0, 0, 0}});
}
