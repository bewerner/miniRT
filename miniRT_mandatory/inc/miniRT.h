/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 15:10:39 by nmihaile          #+#    #+#             */
/*   Updated: 2024/10/25 06:17:53 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "../libft/libft.h"
# include "../MLX42/include/MLX42/MLX42.h"

# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <math.h>
# include <sys/errno.h>

# include "minirt_errors.h"

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

# define MAX_POWER		10000
# define LIGHT_POWER	0.575f
# define EPSILON		0.000001f

typedef enum e_timeraction
{
	TIMER_START,
	TIMER_STOP
}	t_timeraction;

// identifiers for parsing
typedef enum e_identifier
{
	ID_EOF,
	ID_INVALID,
	ID_COMMENT,
	ID_AMBIENT,
	ID_CAMERA,
	ID_POINT_LIGHT,
	ID_SPHERE,
	ID_PLANE,
	ID_CYLINDER
}	t_identifier;

typedef struct s_scene
{
	size_t		obj_cnt;
	size_t		objs_size;
	size_t		light_cnt;
	size_t		light_size;
	size_t		camera_cnt;
}	t_scene;

typedef enum e_mode
{
	MODE_SOLID,
	MODE_NORMAL,
	MODE_PREVIEW
}	t_mode;

typedef union u_color
{
	uint32_t		color;
	struct
	{
		uint8_t		a;
		uint8_t		b;
		uint8_t		g;
		uint8_t		r;
	};
}	t_color;

typedef struct s_ivec2
{
	int32_t		x;
	int32_t		y;
}	t_ivec2;

typedef struct s_vec2
{
	float			x;
	float			y;
}	t_vec2;

typedef struct s_vec3
{
	float			x;
	float			y;
	float			z;
}	t_vec3;

typedef union s_vec4
{
	struct
	{
		float		r;
		float		g;
		float		b;
		float		a;
	};
	struct
	{
		float		x;
		float		y;
		float		z;
		float		w;
	};
}	t_vec4;

typedef struct s_ray
{
	t_vec3			origin;
	t_vec3			dir;
}	t_ray;

typedef enum e_obj_type
{
	OBJ_NONE,
	OBJ_SPHERE,
	OBJ_PLANE,
	OBJ_CYLINDER
}	t_obj_type;

typedef struct s_object		t_object;

typedef struct s_object
{
	t_obj_type		type;
	t_object		*next;
	t_vec3			origin;
	t_vec4			base_color;
}	t_object;

typedef struct s_sphere
{
	t_obj_type		type;
	t_object		*next;
	t_vec3			origin;
	t_vec4			base_color;
	float			radius;
}	t_sphere;

typedef struct s_plane
{
	t_obj_type		type;
	t_object		*next;
	t_vec3			origin;
	t_vec4			base_color;
	t_vec3			normal;
	float			dist;
}	t_plane;

typedef struct s_cylinder
{
	t_obj_type		type;
	t_object		*next;
	t_vec3			origin;
	t_vec4			base_color;
	t_vec3			orientation;
	float			radius;
	float			height;
	t_plane			cap1;
	t_plane			cap2;
}	t_cylinder;

typedef struct s_hitpoint
{
	t_vec3			ray;
	t_vec3			pos;
	t_vec3			normal;
	t_object		*object;
}	t_hitpoint;

typedef struct s_quadratic_formula
{
	float			a;
	float			b;
	float			c;
	float			t0;
	float			t1;
	float			discriminant;
}	t_quadratic_formula;

typedef enum e_light_type
{
	LIGHT_NONE,
	LIGHT_POINT
}	t_light_type;

typedef struct s_light		t_light;

typedef struct s_light
{
	t_light_type	type;
	t_light			*next;
}	t_light;

typedef struct s_point_light
{
	t_light_type	type;
	t_light			*next;
	t_vec3			origin;
	float			ratio;
	float			power;
	float			intensity;
	t_vec4			color;
}	t_point_light;

typedef struct s_movement
{
	t_vec3			acc;
	t_vec3			vel;
}	t_movement;

typedef struct s_camera
{
	t_vec3			origin;
	t_vec3			direction;
	t_vec3			right;
	t_vec3			viewport_light;
	float			yaw;
	float			pitch;
	float			focal_lenth;
	float			fov;
}	t_camera;

typedef struct s_screen
{
	t_vec3			origin;
	t_vec3			pos_null;
	t_vec3			x_dir;
	t_vec3			y_dir;
}	t_screen;

typedef struct s_rt
{
	t_list			*line;
	mlx_t			*mlx;
	mlx_image_t		*canvas;
	t_movement		move;
	t_camera		camera;
	t_screen		screen;
	t_object		*objects;
	t_light			*lights;
	t_vec4			ambient;
	float			clicked;
	int8_t			mouse_buttons_pressed;
	t_ivec2			initial_cursor_pos;
	int				mode;
	int				fd;
}	t_rt;

static const t_ivec2		g_ivec2_zero = (t_ivec2){0, 0};
static const t_vec2			g_vec2_zero = (t_vec2){0, 0};
static const t_vec3			g_vec3_zero = (t_vec3){0, 0, 0};
static const t_vec3			g_vec3_inf = (t_vec3){INFINITY, INFINITY, INFINITY};
static const t_vec4			g_vec4_zero = (t_vec4){{0, 0, 0, 0}};
static const t_vec4			g_vec4_white = (t_vec4){{1, 1, 1, 1}};
static const t_vec4			g_vec4_black = (t_vec4){{0, 0, 0, 1}};
static const t_vec4			g_vec4_transparent = (t_vec4){{0, 0, 0, 0}};
static const t_vec4			g_vec4_mg = (t_vec4){{0.05, 0.05, 0.05, 0}};
static const t_hitpoint		g_hp_inf = (((((((t_hitpoint){
							(t_vec3){INFINITY, INFINITY, INFINITY},
							(t_vec3){INFINITY, INFINITY, INFINITY},
							(t_vec3){0, 0, 0}, NULL}))))));

// main.c

// init_miniRT.c
void			init_mini_rt(char **argv, t_rt *rt);

// cleanup.c
char			*error_msg(t_error error);
void			error(char *message);
void			terminate(char *msg, uint8_t exit_code, t_rt *rt);

// render.c
void			render(t_rt *rt);

// ┌───────┐
// │ Hooks │
// └───────┘

// hooks/init_hooks.c
void			init_hooks(t_rt *rt);

// hooks/key_hook.c
void			key_hook(mlx_key_data_t keydata, void *param);

// ┌────────┐
// │ Parser │
// └────────┘

// parser/loader.c
void			load_scene(char *file, t_rt *rt);

// parser/parser.c
t_error			parse_scene(size_t obj_cnt, size_t light_cnt, t_rt *rt);

// parser/parse_default_objs.c
t_error			parse_ambient(t_rt *rt);
t_error			parse_camera(t_rt *rt);

// parser/parse_lights.c
t_error			parse_point_light(t_point_light *pl, t_rt *rt);

// parser/parse_primitives.c
t_error			parse_sphere(t_sphere *sphere, t_rt *rt);
t_error			parse_plane(t_plane *plane, t_rt *rt);
t_error			parse_cylinder(t_cylinder *cylinder, t_rt *rt);

// parser/parser_utils1.c
void			whitespace_to_space(char *str);
void			ft_skipspace(char **str);
float			ft_atod(char **str, float nbr, int sign_dpoint_dplaces[3]);

// parser/parser_utils2.c
t_identifier	get_identifier(char *line);
float			vr(float nbr, t_vec2 min_max, t_rt *rt);
float			gnv(char **line, t_rt *rt);
char			*prep_line(char *str);

// parser/parser_utils3.c
size_t			obj_size(t_identifier id);
size_t			light_size(t_identifier id);
void			ft_delete_line(char **str);

// ┌────────────┐
// │ Primitives │
// └────────────┘

// primitives/plane.c
t_hitpoint		get_hitpoint_plane(t_ray ray, t_plane *plane);

// primitives/cylinder.c
t_hitpoint		get_hitpoint_cylinder(t_ray ray, t_cylinder *cylinder);

// primitives/sphere.c
t_hitpoint		get_hitpoint_sphere(t_ray ray, t_sphere *sphere);

// primitives/get_diffuse_color.c
t_vec4			get_diffuse_color(t_hitpoint hitpoint, t_rt *rt);

// primitives/get_hitpoint.c
bool			is_obstructed(t_ray ray, t_rt *rt);
t_hitpoint		get_closest_hitpoint(t_ray ray, t_rt *rt);

// ┌───────────┐
// │ Utilities │
// └───────────┘

// utils/color_convert.c
uint32_t		vec4_to_abgr(t_vec4	col, bool dither);
uint32_t		vec4_to_rgba(t_vec4	col, bool dither);

// utils/image.c
void			fill_image(mlx_image_t *image, t_vec4 color);

// utils/vec3_rotate.c
t_vec3			vec3_rotate_x(t_vec3 p, float rad);
t_vec3			vec3_rotate_y(t_vec3 p, float rad);
t_vec3			vec3_rotate_z(t_vec3 p, float rad);

// utils/vec3_utils1.c
t_vec3			vec3_add(const t_vec3 a, const t_vec3 b);
t_vec3			vec3_sub(const t_vec3 a, const t_vec3 b);
t_vec3			vec3_mul(const t_vec3 a, const t_vec3 b);
t_vec3			vec3_scale(const float s, const t_vec3 a);
float			vec3_dot(const t_vec3 a, const t_vec3 b);

// utils/vec3_utils2.c
float			vec3_len(const t_vec3 a);
float			vec3_dist(const t_vec3 a, const t_vec3 b);
t_vec3			vec3_normalize(const t_vec3 a);
t_vec3			vec3_cross(const t_vec3 a, const t_vec3 b);

// utils/vec4_utils1.c
t_vec4			vec4_add(const t_vec4 a, const t_vec4 b);
t_vec4			vec4_sub(const t_vec4 a, const t_vec4 b);
t_vec4			vec4_mul(const t_vec4 a, const t_vec4 b);
t_vec4			vec4_scale(const float s, const t_vec4 a);

#endif