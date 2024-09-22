/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_objects_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 15:26:36 by nmihaile          #+#    #+#             */
/*   Updated: 2024/09/22 15:30:44 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

void	init_gpu_sphere(float *buffer, size_t *i, t_sphere *sphere)
{
	t_gpu_sphere	gpu_sphere;

	gpu_sphere.type = (float)sphere->type;
	gpu_sphere.next_offset = (float)(*i + sizeof(t_gpu_sphere) / sizeof(float));
	gpu_sphere.origin = sphere->origin;
	gpu_sphere.base_color = sphere->base_color;
	gpu_sphere.radius = sphere->radius;
	gpu_sphere.material_idx = (float)sphere->material->index;
	ft_memmove(&buffer[*i], &gpu_sphere, sizeof(t_gpu_sphere));
	*i = gpu_sphere.next_offset;
}

void	init_gpu_plane(float *buffer, size_t *i, t_plane *plane)
{
	t_gpu_plane	gpu_plane;

	gpu_plane.type = (float)plane->type;
	gpu_plane.next_offset = (float)(*i + sizeof(t_gpu_plane) / sizeof(float));
	gpu_plane.origin = plane->origin;
	gpu_plane.base_color = plane->base_color;
	gpu_plane.normal = plane->normal;
	gpu_plane.dist = plane->dist;
	gpu_plane.material_idx = 0;
	if (plane->material != NULL)
		gpu_plane.material_idx = (float)plane->material->index;
	ft_memmove(&buffer[*i], &gpu_plane, sizeof(t_gpu_plane));
	*i = gpu_plane.next_offset;
}

void	init_gpu_cylinder(float *buffer, size_t *i, t_cylinder *cylinder)
{
	t_gpu_cylinder	gpu_cylinder;
	size_t			size_without_caps;
	size_t			i_cap;

	size_without_caps = sizeof(t_gpu_cylinder) - 2 * sizeof(t_gpu_plane);
	gpu_cylinder.type = (float)cylinder->type;
	gpu_cylinder.next_offset
		= (float)(*i + sizeof(t_gpu_cylinder) / sizeof(float));
	gpu_cylinder.origin = cylinder->origin;
	gpu_cylinder.base_color = cylinder->base_color;
	gpu_cylinder.orientation = cylinder->orientation;
	gpu_cylinder.radius = cylinder->radius;
	gpu_cylinder.height = cylinder->height;
	gpu_cylinder.material_idx = (float)cylinder->material->index;
	ft_memmove(&buffer[*i], &gpu_cylinder, size_without_caps);
	i_cap = *i + size_without_caps / sizeof(float);
	init_gpu_plane(buffer, &i_cap, &cylinder->cap1);
	init_gpu_plane(buffer, &i_cap, &cylinder->cap2);
	*i = gpu_cylinder.next_offset;
}

void	init_gpu_hyperboloid(float *buffer, size_t *i,
			t_hyperboloid *hyperboloid)
{
	t_gpu_hyperboloid	gpu_hyperboloid;
	size_t				size;

	size = sizeof(t_gpu_hyperboloid);
	gpu_hyperboloid.type = (float)hyperboloid->type;
	gpu_hyperboloid.next_offset
		= (float)(*i + sizeof(t_gpu_hyperboloid) / sizeof(float));
	gpu_hyperboloid.origin = hyperboloid->origin;
	gpu_hyperboloid.base_color = hyperboloid->base_color;
	gpu_hyperboloid.orientation = hyperboloid->orientation;
	gpu_hyperboloid.height = hyperboloid->height;
	gpu_hyperboloid.a = hyperboloid->a;
	gpu_hyperboloid.b = hyperboloid->b;
	gpu_hyperboloid.c = hyperboloid->c;
	gpu_hyperboloid.shape = hyperboloid->shape;
	gpu_hyperboloid.material_idx = (float)hyperboloid->material->index;
	ft_memmove(&buffer[*i], &gpu_hyperboloid, size);
	*i = gpu_hyperboloid.next_offset;
}
