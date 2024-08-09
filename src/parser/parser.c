/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 11:09:09 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/09 15:02:30 by bwerner          ###   ########.fr       */
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
	rt->objects = (t_object *)ft_calloc(3 + 1, sizeof(t_object));

	rt->objects[0].type = OBJ_PLANE;
	rt->objects[0].origin = (t_vec3){0, 0, 0};
	rt->objects[0].normal = vec3_normalize((t_vec3){0, 0, 1});
	rt->objects[0].base_color = (t_vec4){{1.0, 1.0, 1.0, 1.0}};
	rt->objects[0].dist = vec3_dot(rt->objects[0].origin, rt->objects[0].normal);

	rt->objects[1].type = OBJ_SPHERE;
	rt->objects[1].origin = (t_vec3){0, 1.38, 0};
	rt->objects[1].radius = 1;
	rt->objects[1].base_color = (t_vec4){{0.0, 0.0, 0.0, 1.0}};

	// rt->objects[1].type = OBJ_CYLINDER;
	// rt->objects[1].origin = (t_vec3){0, 0, 2};
	// rt->objects[1].orientation = vec3_normalize((t_vec3){0, -1, 0});
	// rt->objects[1].radius = 0.5;
	// rt->objects[1].base_color = (t_vec4){{1.0, 0.0, 0.0, 1.0}};
	// rt->objects[1].height = 3;
	// rt->objects[1].cap1 = vec3_sub(rt->objects[1].origin, vec3_scale(rt->objects[1].height / 2, rt->objects[1].orientation));
	// rt->objects[1].cap2 = vec3_add(rt->objects[1].origin, vec3_scale(rt->objects[1].height / 2, rt->objects[1].orientation));

	// rt->objects[2].type = OBJ_CYLINDER;
	// rt->objects[2].origin = (t_vec3){0, 0, 2};
	// rt->objects[2].orientation = vec3_normalize((t_vec3){4, 8, 12});
	// rt->objects[2].radius = 0.5;
	// rt->objects[2].base_color = (t_vec4){{1.0, 0.0, 0.0, 1.0}};
	// rt->objects[2].height = 3;
	// rt->objects[2].cap1 = vec3_sub(rt->objects[2].origin, vec3_scale(rt->objects[2].height / 2, rt->objects[2].orientation));
	// rt->objects[2].cap2 = vec3_add(rt->objects[2].origin, vec3_scale(rt->objects[2].height / 2, rt->objects[2].orientation));

	// --------LIGHTS--------
	rt->lights = (t_light *)ft_calloc(5 + 1, sizeof(t_light));

	// rt->lights[0].type = LIGHT_POINT;
	// rt->lights[0].origin = (t_vec3){4, 0, 8};
	// rt->lights[0].radius = 1;
	// rt->lights[0].ratio = 1 * LIGHT_POWER;
	// rt->lights[0].color = (t_vec4){{1.0, 1.0, 1.0, 1}};
	// rt->lights[0].color = vec4_scale(rt->lights[0].intensity, rt->lights[0].color);

	rt->lights[0].type = LIGHT_POINT;
	rt->lights[0].origin = (t_vec3){-3.5, 0, .1};
	rt->lights[0].radius = 0;
	rt->lights[0].ratio = 0.005;
	rt->lights[0].power = rt->lights[0].ratio * MAX_POWER;
	rt->lights[0].intensity = rt->lights[0].power / 38.26f;
	rt->lights[0].color = (t_vec4){{1.0, 1.0, 1.0, 1}};
	// rt->lights[0].color = vec4_scale(rt->lights[0].intensity, rt->lights[0].color);

	rt->lights[1].type = LIGHT_POINT;
	rt->lights[1].origin = (t_vec3){-2, 0, .2};
	rt->lights[1].radius = 0;
	rt->lights[1].ratio = 0.005;
	rt->lights[1].power = rt->lights[1].ratio * MAX_POWER;
	rt->lights[1].intensity = rt->lights[1].power / 38.26f;
	rt->lights[1].color = (t_vec4){{1.0, 1.0, 1.0, 1}};
	// rt->lights[1].color = vec4_scale(rt->lights[1].intensity, rt->lights[1].color);

	rt->lights[2].type = LIGHT_POINT;
	rt->lights[2].origin = (t_vec3){-0.5, 0, .3};
	rt->lights[2].radius = 0;
	rt->lights[2].ratio = 0.005;
	rt->lights[2].power = rt->lights[2].ratio * MAX_POWER;
	rt->lights[2].intensity = rt->lights[2].power / 38.26f;
	rt->lights[2].color = (t_vec4){{1.0, 1.0, 1.0, 1}};
	// rt->lights[2].color = vec4_scale(rt->lights[2].intensity, rt->lights[2].color);

	rt->lights[3].type = LIGHT_POINT;
	rt->lights[3].origin = (t_vec3){1, 0, .4};
	rt->lights[3].radius = 0;
	rt->lights[3].ratio = 0.005;
	rt->lights[3].power = rt->lights[3].ratio * MAX_POWER;
	rt->lights[3].intensity = rt->lights[3].power / 38.26f;
	rt->lights[3].color = (t_vec4){{1.0, 1.0, 1.0, 1}};
	// rt->lights[3].color = vec4_scale(rt->lights[3].intensity, rt->lights[3].color);

	rt->lights[4].type = LIGHT_POINT;
	rt->lights[4].origin = (t_vec3){2.5, 0, .5};
	rt->lights[4].radius = 0;
	rt->lights[4].ratio = 0.005;
	rt->lights[4].power = rt->lights[4].ratio * MAX_POWER;
	rt->lights[4].intensity = rt->lights[4].power / 38.26f;
	rt->lights[4].color = (t_vec4){{1.0, 1.0, 1.0, 1}};
	// rt->lights[4].color = vec4_scale(rt->lights[4].intensity, rt->lights[4].color);

	// rt->ambient = (t_vec4){{.05, .05, .1, 1}};
	// rt->ambient = (t_vec4){{0.1, 0.1, 0.1, 1}};
	rt->ambient = (t_vec4){{0.1, 0.1, 0.1, 1}};

	// --------CAMERA--------
	rt->camera.origin = (t_vec3){0, 0, 16};
	rt->camera.direction = vec3_normalize((t_vec3){0, 0, -1});
	rt->camera.focal_lenth = 1000;	
}
