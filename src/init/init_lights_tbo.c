/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_lights_tbo.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 14:58:13 by nmihaile          #+#    #+#             */
/*   Updated: 2024/10/16 16:54:40 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static size_t	get_gpu_lights_size(t_light *light)
{
	size_t	size;

	size = 0;
	while (light)
	{
		if (light->type == LIGHT_POINT)
			size += sizeof(t_gpu_point_light);
		light = light->next;
	}
	return (size + sizeof(float));
}

static void	init_gpu_point_light(float *buffer, size_t *i, t_point_light *light)
{
	t_gpu_point_light	gpu_light;

	gpu_light.type = (float)light->type;
	gpu_light.next_offset
		= (float)(*i + sizeof(t_gpu_point_light) / sizeof(float));
	gpu_light.origin = light->origin;
	gpu_light.color = light->color;
	gpu_light.power = light->power;
	gpu_light.intensity = light->intensity;
	gpu_light.radius = light->radius;
	ft_memmove(&buffer[*i], &gpu_light, sizeof(t_gpu_point_light));
	*i = gpu_light.next_offset;
}

static void	init_tbo_lights(float *buffer, t_light *light)
{
	size_t	i;

	i = 0;
	while (light)
	{
		if (light->type == LIGHT_POINT)
			init_gpu_point_light(buffer, &i, (t_point_light *)light);
		light = light->next;
	}
}

void	create_tbo_lights(t_rt *rt)
{
	size_t	size;
	float	*buffer;
	GLuint	texture_id;

	size = get_gpu_lights_size(rt->lights);
	buffer = (float *)ft_calloc(1, size);
	if (!buffer)
		terminate("failed to allocate texture buffer", NULL, 1, rt);
	init_tbo_lights(buffer, rt->lights);
	glGenBuffers(1, &rt->tbo_lights_id);
	glBindBuffer(GL_TEXTURE_BUFFER, rt->tbo_lights_id);
	glBufferData(GL_TEXTURE_BUFFER, size, buffer, GL_STATIC_DRAW);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);
	ft_free((void *)&buffer);
	glGenTextures(1, &texture_id);
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_BUFFER, texture_id);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, rt->tbo_lights_id);
}
