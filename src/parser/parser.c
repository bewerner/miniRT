/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 11:09:09 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/07 15:45:05 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

typedef enum e_identifier
{
	ID_INVALID,
	ID_AMBIENT,
	ID_CAMERA,
	ID_LIGHT,
	ID_SPHERE,
	ID_CYLINDER
}	t_identifier;

t_identifier	get_identifier(char *line)
{
	if (line[0] == 'A' || line[0] == 'a')
		return (ID_AMBIENT);
	return (ID_INVALID);
}

// double	get_next_value(char **line)
// {
// 	if (line[0] == 'A' || line[0] == 'a')
// 		return (RT_ID_AMBIENT);
// }


t_error	parse_ambient(char *line, t_rt *rt)
{
	if (line[0] == 'a')
		return (RT_ERROR_AMBIENT_LOWER_CASE);

	rt->ambient = (t_vec4){{.05, .05, .1, 1}};
	return (RT_SUCCESS);
}

static t_error	parse_scene(int fd, t_rt *rt)
{
	t_error	error;
	char	*line;

	error = RT_SUCCESS;
	line = get_next_line(fd);
	while (line)
	{
		if (get_identifier(line) == ID_AMBIENT)
			error = parse_ambient(line, rt);

		free(line);
		if (error)
			return (error);
		line = get_next_line(fd);
	}
	return (RT_SUCCESS);
}

void	load_scene(char *file, t_rt *rt)
{
	int		fd;
	t_error	error;

	fd = open(file, O_RDONLY);
	if (fd == -1)
		terminate("failed to load file", 1, rt);

	error = parse_scene(fd, rt);
	close(fd);
	if (error)
		terminate(error_msg(error), 1, rt);



	// --------OBJECTS--------
	rt->objects = (t_object *)ft_calloc(1 + 1, sizeof(t_object));
	rt->objects[0].type = OBJ_SPHERE;
	rt->objects[0].origin = (t_vec3){0, 0, 0};
	rt->objects[0].radius = 1;
	rt->objects[0].base_color = (t_vec4){{1.0, 1.0, 1.0, 1.0}};

	// --------LIGHTS--------
	rt->lights = (t_light *)ft_calloc(1 + 1, sizeof(t_light));
	rt->lights[0].type = LIGHT_POINT;
	rt->lights[0].origin = (t_vec3){0, 0, 3};
	rt->lights[0].radius = 1;
	rt->lights[0].ratio = 1 * LIGHT_POWER;
	rt->lights[0].color = (t_vec4){{1.0, 1.0, 1.0, 1}};
	rt->lights[0].color = vec4_scale(rt->lights[0].ratio, rt->lights[0].color);

	// --------CAMERA--------
	rt->camera.origin = (t_vec3){0, -8.5, .5};
	rt->camera.direction = (t_vec3){0, 1, 0};
	rt->camera.focal_lenth = 1000;	
}
