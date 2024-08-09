/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 12:38:40 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/09 12:39:13 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

t_identifier	get_identifier(char *line)
{
	if (line == NULL || line[0] == '\0')
		return (ID_EOF);
	if (line[0] == '#')
		return (ID_COMMENT);
	if (line[0] == 'A' || line[0] == 'a')
		return (ID_AMBIENT);
	else if (line[0] == 'C' || line[0] == 'c')
		return (ID_CAMERA);
	else if (line[0] == 'L' || line[0] == 'l')
		return (ID_LIGHT);
	else if (ft_strncmp(line, "SP", 2) == 0 || ft_strncmp(line, "sp", 2) == 0)
		return (ID_SPHERE);
	else if (ft_strncmp(line, "PL", 2) == 0 || ft_strncmp(line, "pl", 2) == 0)
		return (ID_PLANE);
	else if (ft_strncmp(line, "CY", 2) == 0 || ft_strncmp(line, "cy", 2) == 0)
		return (ID_CYLINDER);
	return (ID_INVALID);
}

double	validate_range(double nbr, t_vec2 min_max, t_rt *rt)
{
	if (nbr >= min_max.x && nbr <= min_max.y)
		return (nbr);
	terminate("number not in valid range", 1, rt);
	return (0);
}

double	get_next_value(char **line, t_rt *rt)
{
	double	value;

	ft_skipspace(line);
	if (**line == '\0')
		terminate("Missing value detected", 1, rt);
	value = ft_atod(line, 0, (int[3]){1, -1, 0});
	if (value == NAN || value == INFINITY || value == -INFINITY)
		terminate("invalid number", 1, rt);
	while (*line && (ft_isspace(**line) || **line == ','))
		(*line)++;
	return (value);
}

char *prep_line(char *str)
{
	size_t	len;

	if (str == NULL)
		return (str);
	len = ft_strlen(str);
	if (str[len - 1] == '\n')
		str[len - 1] = '\0';
	if (str && (str[0] == '\0' || !ft_isspace(str[0])))
		return (str);
	len = 0;
	while (ft_isspace(str[len]))
		len++;
	ft_memcpy(str, &str[len], ft_strlen(&str[len]) + 1);
	printf("|%s|\n",str);
	return (str);
}
