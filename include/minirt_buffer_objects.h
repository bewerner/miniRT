/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_buffer_objects.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 18:04:05 by bwerner           #+#    #+#             */
/*   Updated: 2024/10/23 23:38:36 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_BUFFER_OBJECTS_H
# define MINIRT_BUFFER_OBJECTS_H

# include "minirt_vec_types.h"

typedef struct s_gpu_material
{
	t_vec3		color;
	float		metallic;
	float		roughness;
	float		ior;
	float		transmission;
	float		emission_strength;
	t_vec3		emission_color;	
	int			color_texture_id;
	int			normal_map_id;
	float		normal_strength;
	int			metallic_texture_id;
	int			roughness_texture_id;
}	t_gpu_material;

// IDX, TYPE, TEX_IDX, SCALA, COL1, COL2
typedef struct s_gpu_texture
{
	t_vec3		col1;
	int			type;
	t_vec3		col2;
	int			texture_unit;
	float		scale;
	int			padding1;
	int			padding2;
	int			padding3;
}	t_gpu_texture;

typedef struct s_gpu_sphere
{
	float			type;
	float			next_offset;
	t_vec3			origin;
	t_vec3			base_color;
	float			radius;
	t_vec2			uv_scale;
	float			material_idx;
}	t_gpu_sphere;

typedef struct s_gpu_plane
{
	float			type;
	float			next_offset;
	t_vec3			origin;
	t_vec3			base_color;
	t_vec3			normal;
	float			dist;
	t_vec2			uv_scale;
	float			material_idx;
}	t_gpu_plane;

typedef struct s_gpu_cylinder
{
	float			type;
	float			next_offset;
	t_vec3			origin;
	t_vec3			base_color;
	t_vec3			orientation;
	float			radius;
	float			height;
	t_vec2			uv_scale;
	float			material_idx;
	t_gpu_plane		cap1;
	t_gpu_plane		cap2;
}	t_gpu_cylinder;

typedef struct s_gpu_hyperboloid
{
	float			type;
	float			next_offset;
	t_vec3			origin;
	t_vec3			base_color;
	t_vec3			orientation;
	float			height;
	float			a;
	float			b;
	float			c;
	float			shape;
	t_vec2			uv_scale;
	float			material_idx;
	float			padding1;
	float			padding2;
}	t_gpu_hyperboloid;

typedef struct s_gpu_point_light
{
	float			type;
	float			next_offset;
	t_vec3			origin;
	t_vec3			color;
	float			power;
	float			intensity;
	float			radius;
}	t_gpu_point_light;

static const float	g_gizmo_vertices[] = {
	-127, 127, 200, -1, -1, 0.5,
	127, 127, 200, 1, -1, 0.5,
	-127, -127, 200, -1, 1, 0.5,
	127, 127, 200, 1, -1, 0.5,
	127, -127, 200, 1, 1, 0.5,
	-127, -127, 200, -1, 1, 0.5,
	0, 3.31, 0, 1, 0, 0,
	100, 3.31, 0, 1, 0, 0,
	0, -1.6, 0, 1, 0, 0,
	100, 3.31, 0, 1, 0, 0,
	100, -3.31, 0, 1, 0, 0,
	0, -3.31, 0, 1, 0, 0,
	0, 0, -3.31, 1, 0, 0,
	100, 0, -3.31, 1, 0, 0,
	0, 0, 3.31, 1, 0, 0,
	100, 0, -3.31, 1, 0, 0,
	100, 0, 3.31, 1, 0, 0,
	0, 0, 3.31, 1, 0, 0,
	3.31, 0, 0, 0, 1, 0,
	3.31, 100, 0, 0, 1, 0,
	-3.31, 0, 0, 0, 1, 0,
	3.31, 100, 0, 0, 1, 0,
	-3.31, 100, 0, 0, 1, 0,
	-3.31, 0, 0, 0, 1, 0,
	0, 0, -3.31, 0, 1, 0,
	0, 100, -3.31, 0, 1, 0,
	0, 0, 3.31, 0, 1, 0,
	0, 100, -3.31, 0, 1, 0,
	0, 100, 3.31, 0, 1, 0,
	0, 0, 3.31, 0, 1, 0,
	3.31, 0, 0, 0, 0, 1,
	3.31, 0, -100, 0, 0, 1,
	-3.31, 0, 0, 0, 0, 1,
	3.31, 0, -100, 0, 0, 1,
	-3.31, 0, -100, 0, 0, 1,
	-3.31, 0, 0, 0, 0, 1,
	-27, 0, 27, -1, -1, 2,
	27, 0, 27, 1, -1, 2,
	-27, 0, -27, -1, 1, 2,
	27, 0, 27, 1, -1, 2,
	27, 0, -27, 1, 1, 2,
	-27, 0, -27, -1, 1, 2,
	-27, 0, 27, -1, -1, 3,
	27, 0, 27, 1, -1, 3,
	-27, 0, -27, -1, 1, 3,
	27, 0, 27, 1, -1, 3,
	27, 0, -27, 1, 1, 3,
	-27, 0, -27, -1, 1, 3,
	-27, 0, 27, -1, -1, 4,
	27, 0, 27, 1, -1, 4,
	-27, 0, -27, -1, 1, 4,
	27, 0, 27, 1, -1, 4,
	27, 0, -27, 1, 1, 4,
	-27, 0, -27, -1, 1, 4,
	-27, 0, 27, -1, -1, -2,
	27, 0, 27, 1, -1, -2,
	-27, 0, -27, -1, 1, -2,
	27, 0, 27, 1, -1, -2,
	27, 0, -27, 1, 1, -2,
	-27, 0, -27, -1, 1, -2,
	-27, 0, 27, -1, -1, -3,
	27, 0, 27, 1, -1, -3,
	-27, 0, -27, -1, 1, -3,
	27, 0, 27, 1, -1, -3,
	27, 0, -27, 1, 1, -3,
	-27, 0, -27, -1, 1, -3,
	-27, 0, 27, -1, -1, -4,
	27, 0, 27, 1, -1, -4,
	-27, 0, -27, -1, 1, -4,
	27, 0, 27, 1, -1, -4,
	27, 0, -27, 1, 1, -4,
	-27, 0, -27, -1, 1, -4
};

#endif
