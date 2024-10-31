/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bind_buffer2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 16:33:16 by bwerner           #+#    #+#             */
/*   Updated: 2024/10/31 07:18:59 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

void	bind_objects(GLuint shader_program, t_rt *rt)
{
	GLint	uniform_location;

	uniform_location = glGetUniformLocation(shader_program, "objects");
	if (uniform_location == -1)
		terminate("objects not found in shader program", NULL, 1, rt);
	glUniform1i(uniform_location, 1);
}

void	bind_lights(GLuint shader_program, t_rt *rt)
{
	GLint	uniform_location;

	uniform_location = glGetUniformLocation(shader_program, "lights");
	if (uniform_location == -1)
		terminate("lights not found in shader program", NULL, 1, rt);
	glUniform1i(uniform_location, 2);
}

void	bind_agx_lut(GLuint shader_program, t_rt *rt)
{
	GLint	uniform_location;

	uniform_location = glGetUniformLocation(shader_program, "agx_lut");
	if (uniform_location == -1)
		terminate("agx_lut not found in shader program", NULL, 1, rt);
	glUniform1i(uniform_location, 3);
}

void	bind_environment_map(GLuint shader_program, t_rt *rt)
{
	GLint	uniform_location;

	uniform_location = glGetUniformLocation(shader_program, "environment_map");
	if (uniform_location == -1)
		terminate("environment_map not found in shader program", NULL, 1, rt);
	glUniform1i(uniform_location, 4);
}
