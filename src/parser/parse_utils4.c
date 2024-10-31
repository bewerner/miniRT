/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 11:11:08 by nmihaile          #+#    #+#             */
/*   Updated: 2024/10/31 07:18:59 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

static t_identifier	get_identifier_2(char *line)
{
	if (ft_strncmp(line, "A ", 2) == 0)
		return (ID_AMBIENT);
	else if (ft_strncmp(line, "C ", 2) == 0)
		return (ID_CAMERA);
	else if (ft_strncmp(line, "R ", 2) == 0)
		return (ID_RENDER_SETTINGS);
	else if (ft_strncmp(line, "mat ", 4) == 0)
		return (ID_MATERIAL);
	else if (ft_strncmp(line, "tex ", 4) == 0)
		return (ID_TEXTURE);
	else if (ft_strncmp(line, "L ", 2) == 0)
		return (ID_POINT_LIGHT);
	else if (ft_strncmp(line, "l ", 2) == 0)
		return (ID_POINT_LIGHT);
	else if (ft_strncmp(line, "sp ", 3) == 0)
		return (ID_SPHERE);
	else if (ft_strncmp(line, "pl ", 3) == 0)
		return (ID_PLANE);
	else if (ft_strncmp(line, "cy ", 3) == 0)
		return (ID_CYLINDER);
	else if (ft_strncmp(line, "hb ", 3) == 0)
		return (ID_HYPERBOLOID);
	return (ID_INVALID);
}

// parse_utils4.c
t_identifier	get_identifier(char *line)
{
	if (line == NULL || line[0] == '\0')
		return (ID_EOF);
	if (line[0] == '#')
		return (ID_COMMENT);
	return (get_identifier_2(line));
}
