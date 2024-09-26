/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 12:38:40 by nmihaile          #+#    #+#             */
/*   Updated: 2024/09/26 15:54:41 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

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

//	validate_range
//	parse_utils2.c
float	vr(float nbr, t_vec2 min_max, t_rt *rt)
{
	if (nbr >= min_max.x - EPSILON && nbr <= min_max.y + EPSILON)
		return (nbr);
	terminate("number not in valid range", rt->curr_line, 1, rt);
	return (0);
}

//	get_next_value
//	parse_utils2.c
float	gnv(char **line, t_rt *rt)
{
	float	value;

	ft_skipspace(line);
	if (**line == '\0')
		terminate("Missing value detected", rt->curr_line, 1, rt);
	value = ft_atod(line, 0, (int [3]){1, -1, 0});
	if (value == NAN || value == INFINITY || value == -INFINITY)
		terminate("invalid number", rt->curr_line, 1, rt);
	while (*line && (ft_isspace(**line) || **line == ','))
		(*line)++;
	return (value);
}

//	get_next_name_value
//	parser_utils2.c
void	gnv_name(char *name, char **line, t_rt *rt)
{
	size_t	count;

	count = 0;
	ft_skipspace(line);
	if (**line == '\0')
		terminate("Missing value detected", *line, 1, rt);
	if (!ft_isalpha(**line))
		terminate("expecting a name, but name starts not with \
an alphanumeric character", rt->curr_line, 1, rt);
	while (*line && !ft_isspace(**line)
		&& **line != ',' && count < MAX_NAME - 1)
	{
		name[count++] = **line;
		(*line)++;
	}
	name[count] = '\0';
	while (*line && (ft_isspace(**line) || **line == ','))
		(*line)++;
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
