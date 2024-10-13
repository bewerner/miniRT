/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 18:39:29 by nmihaile          #+#    #+#             */
/*   Updated: 2024/10/11 15:21:16 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

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
		free(*str);
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
t_vec3	vn(t_vec3 normal, t_rt *rt)
{
	float	len;

	len = vec3_len(normal);
	if (len >= 1.0f - EPSILON && len <= 1.0f + EPSILON)
		return (normal);
	terminate("normal is not a unit vector", rt->curr_line, 1, rt);
	return (g_vec3_zero);
}
