/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_buffer_objects.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 18:04:05 by bwerner           #+#    #+#             */
/*   Updated: 2024/08/26 20:55:22 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_BUFFER_OBJECTS_H
# define MINIRT_BUFFER_OBJECTS_H

# include "minirt_vec_types.h"

typedef struct s_gpu_sphere
{
	float			type;
	float			next_offset;
	t_vec3			origin;
	t_vec4			base_color;
	float			radius;
}	t_gpu_sphere;

typedef struct s_gpu_plane
{
	float			type;
	float			next_offset;
	t_vec3			origin;
	t_vec4			base_color;
	t_vec3			normal;
	float			dist;
}	t_gpu_plane;

typedef struct s_gpu_cylinder
{
	float			type;
	float			next_offset;
	t_vec3			origin;
	t_vec4			base_color;
	t_vec3			orientation;
	float			radius;
	float			height;
	t_gpu_plane		cap1;
	t_gpu_plane		cap2;
}	t_gpu_cylinder;

#endif
