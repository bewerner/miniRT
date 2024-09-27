/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 15:10:39 by nmihaile          #+#    #+#             */
/*   Updated: 2024/09/28 00:52:52 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# if defined(__APPLE__) && defined(__MACH__)
#  define LUT_PATH	"resources/AgX_LUT3D_P3.png"
#  define MAC_OS	1
# else
#  define LUT_PATH	"resources/AgX_LUT3D_sRGB.png"
#  define MAC_OS	0
# endif

# include "../libft/libft.h"
# include "glad/glad.h"
# include <GLFW/glfw3.h>
# include "stb/stb_image.h"
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

# define MAX_NAME 1024 

# define MAX_IMAGE_TEXTURES 10

typedef enum e_timeraction
{
	TIMER_START,
	TIMER_STOP
}	t_timeraction;

// Typedef Prototypes
typedef struct s_material	t_material;
typedef struct s_texture	t_texture;
typedef struct s_object		t_object;
typedef struct s_light		t_light;

// Identifiers for parsing
typedef enum e_identifier
{
	ID_EOF,
	ID_INVALID,
	ID_COMMENT,
	ID_AMBIENT,
	ID_CAMERA,
	ID_MATERIAL,
	ID_TEXTURE,
	ID_POINT_LIGHT,
	ID_SPHERE,
	ID_PLANE,
	ID_CYLINDER,
	ID_HYPERBOLOID
}	t_identifier;

// Parser Objects for parser.c
typedef struct s_pobjs
{
	// t_material	*curr_mat;
	t_object	*curr_obj;
	t_light		*curr_light;
}				t_pobjs;

typedef struct s_scene_size
{
	size_t		obj_cnt;
	size_t		objs_size;
	size_t		light_cnt;
	size_t		light_size;
	size_t		mat_cnt;
	size_t		tex_cnt;
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

typedef struct s_material
{
	size_t		index;
	t_material	*next;
	char		name[MAX_NAME];
	t_vec3		color;
	float		metallic;
	float		roughness;
	float		ior;
	float		transmission;
	float		emission_strength;
	t_vec3		emission_color;
	int			color_texture_id;
	int			bump_map_id;
}	t_material;

typedef enum e_texture_type
{
	TEX_NONE,
	TEX_IMAGE,
	TEX_CHECKER
}	t_texture_type;

typedef struct s_texture
{
	size_t			index;
	t_texture		*next;
	char			name[MAX_NAME];
	t_texture_type	type;
	char			file[MAX_NAME];
	float			scale;
	t_vec3			col1;
	t_vec3			col2;
	
}	t_texture;

typedef enum e_obj_type
{
	OBJ_NONE,
	OBJ_SPHERE,
	OBJ_PLANE,
	OBJ_CYLINDER,
	OBJ_HYPERBOLOID
}	t_obj_type;

typedef struct s_object
{
	t_obj_type		type;
	t_object		*next;
	t_vec3			origin;
	t_vec3			base_color;
	t_material		*material;
	bool			is_selected;
}	t_object;

typedef struct s_sphere
{
	t_obj_type		type;
	t_object		*next;
	t_vec3			origin;
	t_vec3			base_color;
	t_material		*material;
	bool			is_selected;
	float			radius;
}	t_sphere;

typedef struct s_plane
{
	t_obj_type		type;
	t_object		*next;
	t_vec3			origin;
	t_vec3			base_color;
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
	t_vec3			base_color;
	t_material		*material;
	bool			is_selected;
	t_vec3			orientation;
	float			radius;
	float			height;
	t_plane			cap1;
	t_plane			cap2;
}	t_cylinder;

typedef struct s_hyperboloid
{
	t_obj_type		type;
	t_object		*next;
	t_vec3			origin;
	t_vec3			base_color;
	t_material		*material;
	bool			is_selected;
	t_vec3			orientation;
	float			height;
	float			a;
	float			b;
	float			c;
	float			shape;			
}	t_hyperboloid;

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

typedef struct s_light
{
	t_light_type	type;
	t_light			*next;
	t_vec3			origin;
	t_vec3			color;
}	t_light;

typedef struct s_point_light
{
	t_light_type	type;
	t_light			*next;
	t_vec3			origin;
	t_vec3			color;
	float			power;
	float			intensity;
}	t_point_light;

typedef struct s_movement
{
	t_vec3			acc;
	t_vec3			vel;
	float			speed;
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
	t_vec3			ambient;
	float			aspect_ratio;
	float			debug;
	int				sample_count;
	int				max_samples;
	int				width;
	int				height;
	int				mode;
	int				mac_os;
}	t_ubo;

typedef struct s_rt
{
	int				width;
	int				height;
	float			aspect_ratio;
	float			dpi_scale;
	GLFWwindow		*window;
	double			delta_time;
	GLuint			solid_shader_program;
	GLuint			normal_shader_program;
	GLuint			preview_shader_program;
	GLuint			postprocessing_shader_program;
	GLuint			gizmo_shader_program;
	char			*filename;

	GLuint			vao_screen_id;
	GLuint			vao_gizmo_id;
	GLuint			ubo_rt_id;
	GLuint			ubo_materials_id;
	GLuint			ubo_textures_id;
	GLuint			tbo_objects_id;
	GLuint			tbo_lights_id;	
	GLuint			tbo_agx_lut_id;
	GLuint			fbo_id;
	GLuint			tex_fbo_id;
	GLuint			environment_map_id;
	GLuint			texture_ids[MAX_IMAGE_TEXTURES];

	t_list			*line;
	char			*curr_line;
	t_movement		move;
	t_camera		camera;
	t_screen		screen;
	t_material		*materials;
	t_texture		*textures;
	t_object		*objects;
	t_light			*lights;
	t_vec3			ambient;
	float			ambient_strength;
	char			ambient_env_file[MAX_NAME];
	float			clicked;
	int8_t			mouse_buttons_pressed;
	t_dvec2			initial_cursor_pos;
	t_dvec2			cursor_pos;
	int				mode;
	int				fd;
	bool			cursor_is_settable;
	float			debug;
	int				sample_count;
	int				max_samples;
}	t_rt;

static const t_ivec2		g_ivec2_zero = (t_ivec2){0, 0};
static const t_vec2			g_vec2_zero = (t_vec2){0, 0};
static const t_vec3			g_vec3_zero = (t_vec3){{0, 0, 0}};
static const t_vec3			g_vec3_inf = (t_vec3){{INFINITY, INFINITY, INFINITY}};


static const t_vec3			g_vec3_white = (t_vec3){{1, 1, 1}};
static const t_vec3			g_vec3_black = (t_vec3){{0, 0, 0}};
static const t_vec3			g_vec3_transparent = (t_vec3){{0, 0, 0}};
static const t_vec3			g_vec3_mg = (t_vec3){{0.05, 0.05, 0.05}};

static const t_vec4			g_vec4_zero = (t_vec4){{0, 0, 0, 0}};
static const t_vec4			g_vec4_white = (t_vec4){{1, 1, 1, 1}};
static const t_vec4			g_vec4_black = (t_vec4){{0, 0, 0, 1}};
static const t_vec4			g_vec4_transparent = (t_vec4){{0, 0, 0, 0}};
static const t_vec4			g_vec4_mg = (t_vec4){{0.05, 0.05, 0.05, 0}};
static const t_hitpoint		g_hp_inf = ((((((((t_hitpoint){
							{{INFINITY, INFINITY, INFINITY}},
							{{INFINITY, INFINITY, INFINITY}},
							{{0, 0, 0}}, NULL})))))));

// main.c
t_rt			*get_rt(void);

// init_miniRT.c
void			init_mini_rt(char **argv, t_rt *rt);

// cleanup.c
char			*error_msg(t_error error);
void			error(char *message, char *add_msg);
void			terminate(char *msg, char *add_msg, uint8_t exit_code, t_rt *rt);

// movement.c
void			handle_move_input(t_rt *rt);
void			move_camera(t_rt *rt);

// render.c
// t_vec3			get_pixel_ray(uint32_t x, uint32_t y, t_rt *rt);
// void			render(t_rt *rt);

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
void			cursor_hook(GLFWwindow *window, double cursor_x, double cursor_y);

// hooks/mouse_hook.c
void			mouse_hook(GLFWwindow* window, int button, int action, int mods);

// hooks/resize_hook.c
void			resize_hook(GLFWwindow *window, int width, int height);

// hooks/scroll_hook.c
void 			scroll_hook(GLFWwindow* window, double xoffset, double yoffset);

// hooks/loop_hook.c
// void			loop_hook(void *param);

// ┌──────┐
// │ Init │
// └──────┘

// init/init_agx_lut_tbo.c
void			create_tbo_agx_lut(char *filepath, t_rt *rt);

// init/init_lights_tbo.c
void			create_tbo_lights(t_rt *rt);

// init/load_textures.c
void			load_textures(t_rt *rt);

// init/init_material_tbo.c
void			create_ubo_materials(t_rt *rt);

// init/init_objects_primitives.c
void			init_gpu_sphere(float *buffer, size_t *i, t_sphere *sphere);
void			init_gpu_plane(float *buffer, size_t *i, t_plane *plane);
void			init_gpu_cylinder(float *buffer, size_t *i, t_cylinder *cylinder);
void			init_gpu_hyperboloid(float *buffer, size_t *i, t_hyperboloid *hyperboloid);

// init/init_objects_tbo.c
void			create_tbo_objects(t_rt *rt);

// init/init_texture_ubo.c
void			create_ubo_textures(t_rt *rt);

// ┌────────┐
// │ Parser │
// └────────┘

// parser/loader.c
void			load_scene(char *file, t_rt *rt);

// parser/parser.c
t_error			parse_scene(t_scene_size scene_size, t_rt *rt);

// parser/parse_material.c
t_error			create_materials(size_t mat_cnt, t_rt *rt);

// parser/parse_material2.c
void			create_default_material(size_t mat_cnt, t_material *mat);

// parser/parse_texture.c
t_error			create_textures(size_t tex_cnt, t_rt *rt);

// parser/parse_default_objs.c
t_error			parse_ambient(t_rt *rt);
t_error			parse_camera(t_rt *rt);

// parser/parse_lights.c
t_error			parse_point_light(t_point_light *pl, t_rt *rt);

// parser/parse_primitives.c
t_error			parse_sphere(t_sphere *sphere, t_rt *rt);
t_error			parse_plane(t_plane *plane, t_rt *rt);
t_error			parse_cylinder(t_cylinder *cylinder, t_rt *rt);

// parser/parse_primitives2.c
t_material		*get_next_material(char *line, t_rt *rt);
void			set_color_and_material(t_vec3 *col, t_material **mat, char *line, t_rt *rt);
t_error			parse_hyperboloid(t_hyperboloid *hb, t_rt *rt);

// parser/parser_utils1.c
t_identifier	get_identifier(char *line);
float			ft_atod(char **str, float nbr, int sign_dpoint_dplaces[3]);
void			next_lst_item(t_list **lst);
bool			next_is_name(char *line);

// parser/parser_utils2.c
float			vr(float nbr, t_vec2 min_max, t_rt *rt);
float			gnv(char **line, t_rt *rt);
void			gnv_name(char *name, char **line, t_rt *rt);
char			*prep_line(char *str);

// parser/parser_utils3.c
size_t			obj_size(t_identifier id);
size_t			light_size(t_identifier id);
void			ft_delete_line(char **str);

// parser/parser_validations.c
void			verify_material_uniqueness(t_rt *rt);
void			verify_texture_uniqueness(t_rt *rt);
// select.c
// void			unselect_all(t_object *obj);
// void			rt_select(GLFWwindow* window, t_rt *rt);

// ┌────────────┐
// │ Primitives │
// └────────────┘

// primitives/plane.c
t_hitpoint		get_hitpoint_plane(t_ray ray, t_plane *plane);

// primitives/cylinder.c
t_hitpoint		get_hitpoint_cylinder(t_ray ray, t_cylinder *cylinder);

// primitives/sphere.c
t_hitpoint		get_hitpoint_sphere(t_ray ray, t_sphere *sphere);

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

// shader/bind_buffer.c
void			bind_framebuffer_texture(GLuint shader_program, t_rt *rt);
void			bind_objects(GLuint shader_program, t_rt *rt);
void			bind_lights(GLuint shader_program, t_rt *rt);
void			bind_agx_lut(GLuint shader_program, t_rt *rt);
void			bind_environment_map(GLuint shader_program, t_rt *rt);
void			bind_texture_units(GLuint shader_program, t_rt *rt);
void			bind_material_ubo(GLuint shader_program, t_rt *rt);
void			bind_texture_ubo(GLuint shader_program, t_rt *rt);


// shader/run_shader.c
void			draw_gizmo(t_rt *rt);
void			postprocess_raw_image(t_rt *rt);
void			render_raw_image(t_rt *rt);

// ┌───────────┐
// │ Utilities │
// └───────────┘

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

// utils/time.c
void			ft_timer(t_timeraction action, char *msg);

#endif
