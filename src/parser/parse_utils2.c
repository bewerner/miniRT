/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 12:38:40 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/13 20:11:16 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static bool	check_id(char *id, char *s1, char *s2)
{
	if (ft_strncmp(id, s1, ft_strlen(s1)) == 0
		|| ft_strncmp(id, s2, ft_strlen(s2)) == 0)
		return (true);
	return (false);
}

t_identifier	get_identifier(char *line)
{
	if (line == NULL || line[0] == '\0')
		return (ID_EOF);
	if (line[0] == '#')
		return (ID_COMMENT);
	if (ft_strncmp(line, "A ", 2) == 0)
		return (ID_AMBIENT);
	else if (ft_strncmp(line, "C ", 2) == 0)
		return (ID_CAMERA);
	else if (check_id(line, "L ", "l "))
		return (ID_POINT_LIGHT);
	else if (check_id(line, "SP ", "sp "))
		return (ID_SPHERE);
	else if (check_id(line, "PL ", "pl "))
		return (ID_PLANE);
	else if (check_id(line, "CY ", "cy "))
		return (ID_CYLINDER);
	return (ID_INVALID);
}

//	validate_range
float	vr(float nbr, t_vec2 min_max, t_rt *rt)
{
	if (nbr >= min_max.x - EPSILON && nbr <= min_max.y + EPSILON)
		return (nbr);
	terminate("number not in valid range", 1, rt);
	return (0);
}

//	get_next_value
float	gnv(char **line, t_rt *rt)
{
	float	value;

	ft_skipspace(line);
	if (**line == '\0')
		terminate("Missing value detected", 1, rt);
	value = ft_atod(line, 0, (int [3]){1, -1, 0});
	if (value == NAN || value == INFINITY || value == -INFINITY)
		terminate("invalid number", 1, rt);
	while (*line && (ft_isspace(**line) || **line == ','))
		(*line)++;
	return (value);
}

char	*prep_line(char *str)
{
	size_t	len;

	if (str == NULL)
		return (str);
	whitespace_to_space(str);
	len = ft_strlen(str);
	if (str && (str[0] == '\0' || !ft_isspace(str[0])))
		return (str);
	len = 0;
	while (ft_isspace(str[len]))
		len++;
	ft_memcpy(str, &str[len], ft_strlen(&str[len]) + 1);
	return (str);
}
