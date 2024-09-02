/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 15:10:39 by nmihaile          #+#    #+#             */
/*   Updated: 2024/09/02 10:55:43 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "../libft/libft.h"
# include "glad/glad.h"
# include <GLFW/glfw3.h>
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <math.h>
# include <sys/errno.h>
# include <stdbool.h>

# include "minirt_errors.h"
# include "minirt_vec_types.h"
# include "minirt_buffer_objects.h"

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

# define SWAP_INTERVAL	1

# define MAX_POWER		10000
# define LIGHT_POWER	0.575f
# define EPSILON		0.000001f

#define MAX_MATERIAL_NAME 256 

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
	ID_MATERIAL,
	ID_POINT_LIGHT,
	ID_SPHERE,
	ID_PLANE,
	ID_CYLINDER
}	t_identifier;

typedef struct s_scene_size
{
	size_t		obj_cnt;
	size_t		objs_size;
	size_t		light_cnt;
	size_t		light_size;
	size_t		mat_cnt;
}	t_scene_size;

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

typedef struct s_ray
{
	t_vec3			origin;
	t_vec3			dir;
}	t_ray;

typedef struct s_material	t_material;

typedef struct	s_material
{
	size_t		index;
	t_material	*next;
	char		name[MAX_MATERIAL_NAME];
	t_vec4		color;
	float		metallic;
	float		roughness;
	float		ior;
	float		transmission;
	float		emission_strength;
	t_vec4		emission_color;
}	t_material;

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
	t_material		*material;
	bool			is_selected;
}	t_object;

typedef struct s_sphere
{
	t_obj_type		type;
	t_object		*next;
	t_vec3			origin;
	t_vec4			base_color;
	t_material		*material;
	bool			is_selected;
	float			radius;
}	t_sphere;

typedef struct s_plane
{
	t_obj_type		type;
	t_object		*next;
	t_vec3			origin;
	t_vec4			base_color;
	t_material		*material;
	bool			is_selected;
	t_vec3			normal;
	float			dist;
}	t_plane;

typedef struct s_cylinder
{
	t_obj_type		type;
	t_object		*next;
	t_vec3			origin;
	t_vec4			base_color;
	t_material		*material;
	bool			is_selected;
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
	// t_ray			incident_ray;
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
	t_vec3			origin;
	t_vec4			color;
}	t_light;

typedef struct s_point_light
{
	t_light_type	type;
	t_light			*next;
	t_vec3			origin;
	t_vec4			color;
	float			ratio;
	float			power;
	float			intensity;
}	t_point_light;

typedef struct s_movement
{
	t_vec3			acc;
	t_vec3			vel;
}	t_movement;

typedef struct s_camera
{
	t_vec3			origin;
	float			yaw;
	t_vec3			direction;
	float			pitch;
	t_vec3			right;
	float			focal_length;
	t_vec3			viewport_light;
	float			fov;
}	t_camera;

typedef struct s_screen
{
	t_vec3			origin;
	t_vec3			pos_null;
	t_vec3			x_dir;
	t_vec3			y_dir;
}	t_screen;

typedef struct s_ubo
{
	t_camera		camera;
	t_vec4			ambient;
	float			aspect_ratio;
}	t_ubo;

typedef struct s_rt
{
	int				width;
	int				height;
	GLFWwindow		*window;
	double			delta_time;
	GLuint			shader_program;
	GLuint			solid_shader_program;
	GLuint			normal_shader_program;
	GLuint			vertex_array_object;
	GLuint			ubo_rt_id;
	
	// GLuint			materials_texture_id;	// optional
	GLuint			ubo_materials_id;

	GLuint			objects_texture_id;		// optional
	GLuint			tbo_objects_id;

	GLuint			lights_texture_id;		// optional
	GLuint			tbo_lights_id;

	t_list			*line;
	t_movement		move;
	t_camera		camera;
	t_screen		screen;
	t_material		*materials;
	t_object		*objects;
	t_light			*lights;
	t_vec4			ambient;
	float			clicked;
	int8_t			mouse_buttons_pressed;
	t_vec2			initial_cursor_pos;
	int				mode;
	int				fd;
	bool			cursor_is_settable;
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
static const t_hitpoint		g_hp_inf = ((((((((t_hitpoint){
							{INFINITY, INFINITY, INFINITY},
							{INFINITY, INFINITY, INFINITY},
							{0, 0, 0}, NULL})))))));

// main.c
t_rt			*get_rt(void);

// init_miniRT.c
void			init_mini_rt(char **argv, t_rt *rt);

// cleanup.c
char			*error_msg(t_error error);
void			error(char *message);
void			terminate(char *msg, uint8_t exit_code, t_rt *rt);

// movement.c
void			handle_move_input(t_rt *rt);
void			move_camera(t_rt *rt);

// render.c
t_vec3			get_pixel_ray(uint32_t x, uint32_t y, t_rt *rt);
void			render(t_rt *rt);

// update.c
void			update(t_rt *rt);

// ┌───────┐
// │ Hooks │
// └───────┘

// hooks/init_hooks.c
void			init_hooks(t_rt *rt);

// hooks/key_hook.c
void			reset_camera(t_camera *camera);
void			key_hook(GLFWwindow *window, int key, int scancode, int action, int mods);

// hooks/cursor_hook.c
void			cursor_hook(GLFWwindow* window, double cursor_x, double cursor_y);

// hooks/mouse_hook.c
void			mouse_hook(GLFWwindow* window, int button, int action, int mods);

// hooks/resize_hook.c
void			resize_hook(GLFWwindow *window, int width, int height);

// hooks/loop_hook.c
void			loop_hook(void *param);

// ┌──────┐
// │ Init │
// └──────┘

// init/init_material_tbo.c
void			create_ubo_materials(t_rt *rt);

// ┌────────┐
// │ Parser │
// └────────┘

// parser/loader.c
void			load_scene(char *file, t_rt *rt);

// parser/parser.c
t_error			parse_scene(size_t mat_cnt, size_t obj_cnt, size_t light_cnt, t_rt *rt);

// parser/parse_material.c
void			verify_material_uniqueness(t_rt *rt);
void			create_default_material(size_t mat_cnt, t_material *mat);
t_error			parse_material(t_material *mat, t_rt *rt);

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
float			ft_atod(char **str, float nbr, int sign_dpoint_dplaces[3]);
void			next_lst_item(t_list **lst);

// parser/parser_utils2.c
t_identifier	get_identifier(char *line);
float			vr(float nbr, t_vec2 min_max, t_rt *rt);
float			gnv(char **line, t_rt *rt);
char			*prep_line(char *str);

// parser/parser_utils3.c
size_t			obj_size(t_identifier id);
size_t			light_size(t_identifier id);
void			ft_delete_line(char **str);

// select.c
// void			unselect_all(t_object *obj);
// void			rt_select(t_rt *rt);

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
t_vec4			get_solid_color(t_hitpoint hitpoint, t_rt *rt);
t_vec4			get_normal_color(t_hitpoint hitpoint, t_rt *rt);

// primitives/get_specular_color.c
t_vec4			get_specular_color(t_hitpoint hp, t_rt *rt);

// primitives/get_reflection_color.c
t_vec4			get_reflection_color(t_hitpoint hp, t_rt *rt);

// primitives/get_hitpoint.c
bool			is_obstructed(t_ray ray, t_rt *rt);
t_hitpoint		get_closest_hitpoint(t_ray ray, t_rt *rt);

// ┌────────┐
// │ Shader │
// └────────┘

// shader/shader_assembler.c
char			*assemble_shader_source(const char *file);

// shader/shader_compiler.c
size_t			file_size(const char *file);
int				is_import(char *str);
void			extract_shader_path(char *shader_path, const char *file);
void			prepares_import_filename(char *filename, char *str, char *shader_path);
GLuint			compile_shader_src(GLenum shader_type, const char *shader_src);

// shader/shader_program.c
GLuint			create_shader_program(const char *vert, const char *freg, t_rt *rt);

// ┌───────────┐
// │ Utilities │
// └───────────┘

// utils/color_convert.c
uint32_t		rgba(int r, int g, int b, int a);
uint32_t		vec4_to_abgr(t_vec4	col, bool dither);
uint32_t		vec4_to_rgba(t_vec4	col, bool dither);

// utils/ray_utils.c
t_vec3			create_bounce_dir(t_vec3 incoming_dir, t_vec3 normal);

// utils/string_utils.c
void			whitespace_to_space(char *str);
void			ft_skipspace(char **str);
int				ft_strcmp(const char *s1, const char *s2);
void			ft_terminate_after_word(char *str);

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

// utils/time.c
void			ft_timer(t_timeraction action, char *msg);

#endif
