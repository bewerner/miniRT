/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_buffer_objects.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 18:04:05 by bwerner           #+#    #+#             */
/*   Updated: 2024/09/04 16:39:05 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_BUFFER_OBJECTS_H
# define MINIRT_BUFFER_OBJECTS_H

# include "minirt_vec_types.h"

typedef struct s_gpu_material
{
	t_vec4		color;
	float		metallic;
	float		roughness;
	float		ior;
	float		transmission;
	t_vec4		emission_color;	
	float		emission_strength;
	float		padding1;
	float		padding2;
	float		padding3;
}	t_gpu_material;

typedef struct s_gpu_sphere
{
	float			type;
	float			next_offset;
	t_vec3			origin;
	t_vec4			base_color;
	float			radius;
	float			material_idx;
}	t_gpu_sphere;

typedef struct s_gpu_plane
{
	float			type;
	float			next_offset;
	t_vec3			origin;
	t_vec4			base_color;
	t_vec3			normal;
	float			dist;
	float			material_idx;
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
	float			material_idx;
	t_gpu_plane		cap1;
	t_gpu_plane		cap2;
}	t_gpu_cylinder;

typedef struct s_gpu_hyperboloid
{
	float			type;
	float			next_offset;
	t_vec3			origin;
	t_vec4			base_color;
	float			a;
	float			b;
	float			c;
	float			shape;
	float			material_idx;
	float			padding1;
	float			padding2;
}	t_gpu_hyperboloid;

typedef struct s_gpu_point_light
{
	float			type;
	float			next_offset;
	t_vec3			origin;
	t_vec4			color;
	float			ratio;
	float			power;
	float			intensity;
}	t_gpu_point_light;

#endif
