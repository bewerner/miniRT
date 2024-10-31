/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 12:38:40 by nmihaile          #+#    #+#             */
/*   Updated: 2024/10/31 07:18:59 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

//	validate_range
//	parse_utils2.c
float	vr(float nbr, t_vec2 min_max, t_rt *rt)
{
	if (nbr >= min_max.x - EPSILON && nbr <= min_max.y + EPSILON)
		return (nbr);
	rt->invalid_range = true;
	rt->invalid_value = nbr;
	rt->allowed_range = min_max;
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

static void	check_filepath_size(char *line, t_rt *rt)
{
	size_t	len;
	char	msg[1024];
	char	*nbr;

	len = ft_strlen(line);
	if (len > MAX_NAME)
	{
		nbr = ft_itoa(MAX_NAME);
		if (nbr == NULL)
			terminate("failed to allocate memory",
				rt->curr_line, 1, rt);
		ft_memset(msg, 0, 1024);
		ft_strlcat(msg, "path and filename longer than ", 1024);
		ft_strlcat(msg, nbr, 1024);
		ft_strlcat(msg, " characters", 1024);
		ft_free((void *)&nbr);
		terminate(msg, rt->curr_line, 1, rt);
	}
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
	check_filepath_size(*line, rt);
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
