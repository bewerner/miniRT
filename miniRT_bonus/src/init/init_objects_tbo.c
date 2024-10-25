/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_objects_tbo.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 15:11:44 by nmihaile          #+#    #+#             */
/*   Updated: 2024/10/16 16:55:07 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static size_t	get_gpu_objects_size(t_object *object)
{
	size_t	size;

	size = 0;
	while (object)
	{
		if (object->type == OBJ_SPHERE)
			size += sizeof(t_gpu_sphere);
		else if (object->type == OBJ_PLANE)
			size += sizeof(t_gpu_plane);
		else if (object->type == OBJ_CYLINDER)
			size += sizeof(t_gpu_cylinder);
		else if (object->type == OBJ_HYPERBOLOID)
			size += sizeof(t_gpu_hyperboloid);
		object = object->next;
	}
	return (size + sizeof(float));
}

static void	init_tbo_objects(float *buffer, t_object *object)
{
	size_t	i;

	i = 0;
	while (object)
	{
		if (object->type == OBJ_SPHERE)
			init_gpu_sphere(buffer, &i, (t_sphere *)object);
		else if (object->type == OBJ_PLANE)
			init_gpu_plane(buffer, &i, (t_plane *)object);
		else if (object->type == OBJ_CYLINDER)
			init_gpu_cylinder(buffer, &i, (t_cylinder *)object);
		else if (object->type == OBJ_HYPERBOLOID)
			init_gpu_hyperboloid(buffer, &i, (t_hyperboloid *)object);
		object = object->next;
	}
}

void	create_tbo_objects(t_rt *rt)
{
	size_t	size;
	float	*buffer;
	GLuint	texture_id;

	size = get_gpu_objects_size(rt->objects);
	buffer = (float *)ft_calloc(1, size);
	if (!buffer)
		terminate("failed to allocate texture buffer", NULL, 1, rt);
	init_tbo_objects(buffer, rt->objects);
	glGenBuffers(1, &rt->tbo_objects_id);
	glBindBuffer(GL_TEXTURE_BUFFER, rt->tbo_objects_id);
	glBufferData(GL_TEXTURE_BUFFER, size, buffer, GL_STATIC_DRAW);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);
	ft_free((void *)&buffer);
	glGenTextures(1, &texture_id);
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_BUFFER, texture_id);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, rt->tbo_objects_id);
}
