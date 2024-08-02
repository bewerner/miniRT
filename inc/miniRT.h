/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 15:10:39 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/02 22:38:28 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "../libft/libft.h"
# include "../MLX42/include/MLX42/MLX42.h"

# include <stdio.h>
# include <math.h>
# include <unistd.h>
# include <sys/errno.h>

# define WINDOW_WIDTH	1280
# define WINDOW_HEIGHT	720

# define VEC2_INT_ZERO	(t_vec2_32int){0, 0}
# define VEC2_ZERO		(t_vec2){0, 0}
# define VEC3_ZERO		(t_vec3){0, 0, 0}
# define VEC4_ZERO		(t_vec4){{0, 0, 0, 0}}
# define VEC4_BLACK		(t_vec4){{0, 0, 0, 1}}

typedef enum e_timeraction
{
	TIMER_START,
	TIMER_STOP
}	t_timeraction;

typedef union		u_color
{
	uint32_t		color;
	struct
	{
		uint8_t		a;
		uint8_t		b;
		uint8_t		g;
		uint8_t		r;
	};
}					t_color;

typedef struct 		s_vec2
{
	double			x;
	double			y;
}					t_vec2;

typedef struct 		s_vec3
{
	double			x;
	double			y;
	double			z;
}					t_vec3;

typedef union		s_vec4
{
	struct
	{
		double		r;
		double		g;
		double		b;
		double		a;
	};
	struct
	{
		double		x;
		double		y;
		double		z;
		double		w;
	};
}					t_vec4;

typedef enum e_obj_type
{
	OBJ_SPHERE,
	OBJ_PLANE,
	OBJ_CYLINDER
}	t_obj_type;

typedef struct		s_object
{
	t_obj_type		type;
	t_vec3			origin;
	t_vec3			normal;
	double			radius;
	double			height;
}					t_object;

typedef struct		s_camera
{
	t_vec3			position;
	t_vec3			direction;
	double			focal_lenth;
}					t_camera;

typedef struct		s_rt
{
	mlx_t			*mlx;
	mlx_image_t		*canvas;
	t_camera		camera;
	t_object		*objects;
}					t_rt;

// cleanup.c
void	error(char *message);
void	terminate(char* message, uint8_t exit_code, t_rt *rt);

// hooks.c
void	init_hooks(t_rt *rt);

// render.c
void	render(t_rt *rt);

// utils/vec3_utils1.c
t_vec3	vec3_add(const t_vec3 a, const t_vec3 b);
t_vec3	vec3_sub(const t_vec3 a, const t_vec3 b);
t_vec3	vec3_mul(const t_vec3 a, const t_vec3 b);
t_vec3	vec3_scale(const double s, const t_vec3 a);
double	vec3_dot(const t_vec3 a, const t_vec3 b);

// utils/vec3_utils2.c
double	vec3_len(const t_vec3 a);
t_vec3	vec3_normalize(const t_vec3 a);

#endif
