/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 15:10:39 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/04 21:10:42 by nmihaile         ###   ########.fr       */
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

# define LIGHTGRAY		"\033[97m"
# define DARKGRAY		"\033[90m"
# define LIGHTRED		"\033[91m"
# define LIGHTGREEN		"\033[92m"
# define LIGHTYELLOW	"\033[93m"
# define LIGHTBLUE		"\033[94m"
# define LIGHTMAGENTA	"\033[95m"
# define LIGHTCYAN		"\033[96m"
# define RESET			"\033[0m"

# define WINDOW_WIDTH	1280
# define WINDOW_HEIGHT	720

# define IVEC2_ZERO			(t_ivec2){0, 0}
# define VEC2_ZERO			(t_vec2){0, 0}
# define VEC3_ZERO			(t_vec3){0, 0, 0}
# define VEC3_INF			(t_vec3){INFINITY, INFINITY, INFINITY}
# define VEC4_ZERO			(t_vec4){{0, 0, 0, 0}}
# define VEC4_WHITE			(t_vec4){{1, 1, 1, 1}}
# define VEC4_BLACK			(t_vec4){{0, 0, 0, 1}}
# define VEC4_TRANSPARENT	(t_vec4){{0, 0, 0, 0}}

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

typedef struct s_ivec2
{
	int32_t		x;
	int32_t		y;
}				t_ivec2;

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

typedef struct		s_ray
{
	t_vec3			origin;
	t_vec3			dir;
}					t_ray;

typedef enum e_obj_type
{
	OBJ_NONE,
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

typedef struct		s_hitpoint
{
	t_vec3			ray;
	t_vec3			pos;
	t_object		*object;
}					t_hitpoint;

typedef struct		s_light
{
	// t_light_type	type;
	// t_vec3		dir;
	t_vec3			origin;
	double			radius;
	double			ratio;
	t_vec4			color;
}					t_light;

typedef struct		s_movement
{
	t_vec3			acc;
	t_vec3			vel;
}					t_movement;

typedef struct		s_camera
{
	t_vec3			origin;
	t_vec3			direction;
	t_vec3			right;
	double			yaw;
	double			pitch;
	double			focal_lenth;
}					t_camera;

typedef struct		s_screen
{
	t_vec3			origin;
	t_vec3			pos_null;
	t_vec3			x_dir;
	t_vec3			y_dir;
}					t_screen;

typedef struct		s_rt
{
	mlx_t			*mlx;
	mlx_image_t		*canvas;
	t_movement		move;
	t_camera		camera;
	t_screen		screen;
	t_object		*objects;
	t_light			*lights;
	t_vec4			ambient;
	double			clicked;
	int8_t			mouse_buttons_pressed;
	t_ivec2			initial_cursor_pos;

}					t_rt;

// cleanup.c
void	error(char *message);
void	terminate(char* message, uint8_t exit_code, t_rt *rt);

// hooks.c
void	init_hooks(t_rt *rt);

// movement.c
void	handle_move_input(t_rt *rt);
void	move_camera(t_rt *rt);

// render.c
t_hitpoint	get_closest_hitpoint(t_ray ray, t_rt *rt);
void		render(t_rt *rt);

// primitives/sphere.c
t_hitpoint	get_hitpoint_sphere(t_ray ray, t_object *object);
t_vec4		get_diffuse_color_sphere(t_hitpoint hitpoint, t_rt *rt);

// color_convert.c
uint32_t	combine_rgba(int r, int g, int b, int a);
uint32_t	vec4_to_rgba(t_vec4	col);

// utils/vec3_rotate.c
t_vec3	vec3_rotate_x(t_vec3 p, double rad);
t_vec3	vec3_rotate_y(t_vec3 p, double rad);
t_vec3	vec3_rotate_z(t_vec3 p, double rad);

// utils/vec3_utils1.c
t_vec3	vec3_add(const t_vec3 a, const t_vec3 b);
t_vec3	vec3_sub(const t_vec3 a, const t_vec3 b);
t_vec3	vec3_mul(const t_vec3 a, const t_vec3 b);
t_vec3	vec3_scale(const double s, const t_vec3 a);
double	vec3_dot(const t_vec3 a, const t_vec3 b);

// utils/vec3_utils2.c
double	vec3_len(const t_vec3 a);
t_vec3	vec3_normalize(const t_vec3 a);
t_vec3	vec3_cross(const t_vec3 a, const t_vec3 b);

// utils/vec4_utils1.c
t_vec4	vec4_add(const t_vec4 a, const t_vec4 b);
t_vec4	vec4_scale(const double s, const t_vec4 a);

// utils/time.c
void	ft_timer(t_timeraction action, char *msg);

#endif
