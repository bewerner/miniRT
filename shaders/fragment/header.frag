#define M_PI		3.1415926535897932384626433832795
#define EPSILON		0.0001
#define MIN_FLOAT	1.175494351e-38
#define INF			(1.0 / 0.0)
#define VEC2_INF	vec2(INF, INF)
#define VEC3_INF	vec3(INF, INF, INF)
#define VEC3_BLACK	vec3(0, 0, 0)
#define VEC3_WHITE	vec3(1, 1, 1)
#define VEC4_BLACK	vec4(0, 0, 0, 1)
#define HP_INF		t_hitpoint(false, VEC3_INF, VEC3_INF, VEC3_INF, VEC3_INF, VEC3_INF, VEC3_INF, VEC2_INF, VEC3_BLACK, -1)

#define MODE_SOLID		0
#define MODE_NORMAL		1
#define MODE_PREVIEW	2

#define MAX_IMAGE_TEXTURES 10

#define TEX_NONE		0
#define TEX_IMAGE		1
#define TEX_CHECKER		2

int	g_seed = 0;

struct	t_ray
{
	vec3	origin;
	vec3	dir;
};

struct t_hitpoint
{
	bool			hit;
	vec3			ray;
	vec3			pos;
	vec3			normal;
	vec3			object_normal;
	vec3			tangent;
	vec3			bitangent;
	vec2			uv;
	vec3			color;
	int				material_idx;
};

#define OBJ_NONE		0
#define OBJ_SPHERE		1
#define OBJ_PLANE		2
#define OBJ_CYLINDER	3
#define OBJ_HYPERBOLOID	4

struct t_sphere
{
	int				type;
	int				next_offset;
	vec3			origin;
	vec3			base_color;
	float			radius;
	int				material_idx;
};

struct t_plane
{
	int				type;
	int				next_offset;
	vec3			origin;
	vec3			base_color;
	vec3			normal;
	float			dist;
	int				material_idx;
};

struct t_cylinder
{
	int				type;
	int				next_offset;
	vec3			origin;
	vec3			base_color;
	vec3			orientation;
	float			radius;
	float			height;
	int				material_idx;
	t_plane			cap1;
	t_plane			cap2;
};

struct t_hyperboloid
{
	int				type;
	int				next_offset;
	vec3			origin;
	vec3			base_color;
	vec3			orientation;
	float			height;
	float			a;
	float			b;
	float			c;
	float			shape;
	int				material_idx;
};

#define LIGHT_NONE		0
#define LIGHT_POINT		1

struct t_point_light
{
	int				type;
	int				next_offset;
	vec3			origin;
	vec3			color;
	// float			ratio;
	float			power;
	float			intensity;
	float			radius;
};

struct t_material
{
	vec3		color;
	float		metallic;
	float		roughness;
	float		ior;
	float		transmission;
	float		emission_strength;
	vec3		emission_color;
	int			color_tex_idx;
	int			normal_map_idx;
	float		normal_strength;
	int			padding2;
	int			padding3;
};

struct t_texture
{
	vec3		col1;
	int			type;
	vec3		col2;
	int			texture_unit;
	float		scale;
	int			padding1;
	int			padding2;
	int			padding3;
};

	// int			type;
	// t_vec3		col1;
	// int			texture_unit;
	// t_vec3		col2;
	// float		scale;
	// int			padding1;
	// int			padding2;
	// int			padding3;

struct t_camera
{
	vec3			origin;
	float			yaw;
	vec3			direction;
	float			pitch;
	vec3			right;
	float			focal_length;
	vec3			viewport_light;
	float			fov;
};

struct t_rt
{
	t_camera		camera;
	vec3			ambient;
	float			aspect_ratio;
	float			debug;
	int				sample_count;
	int				max_samples;
	int				width;
	int				height;
	int				mode;
	int				mac_os;
};

// ┌───────────────────────────┐
// │ in/out and buffer objects │
// └───────────────────────────┘

layout(std140) uniform u_rt
{
	t_rt			rt;
};

layout(std140) uniform u_materials
{
	t_material		materials[100];
};

layout(std140) uniform u_textures
{
	t_texture		textures[100];
};

in vec2 coord;
uniform samplerBuffer	objects;
uniform samplerBuffer	lights;
uniform sampler3D		agx_lut;
uniform sampler2D		cumulative_render_buffer;
uniform sampler2D		environment_map;
uniform sampler2D		texture_units[MAX_IMAGE_TEXTURES];

out vec3 FragColor;

// AgX.frag
vec3			to_agx(vec3 col);

// checker_texture.frag
vec3			get_checker_color(int tex_idx, t_hitpoint hitpoint);

// noise_texture.frag
vec3			get_noise_color(int tex_idx, t_hitpoint hitpoint);

// get_sky_color.frag
vec3			get_random_cosine_weighted_hemisphere_direction(vec3 hemisphere_normal);
vec3			get_real_random_hemisphere_direction(t_hitpoint hitpoint);
vec3			get_environment_map_color(vec3 direction);
vec3			get_sky_color(t_hitpoint hitpoint);
vec3			get_sky_color_from_ray(t_ray ray);

// pbr.frag
vec3			compute_pbr(t_ray ray);

// ┌─────────┐
// │ Objects │
// └─────────┘

// objects/object_utils.frag
int				next_object_type(inout int i);
int				next_light_type(inout int i);

// objects/sphere.frag
t_hitpoint		get_hitpoint_sphere(t_ray ray, t_sphere sphere, bool init_all);
t_sphere		get_sphere(int offset);

// objects/plane.frag
bool			is_near_zero(float value);
t_hitpoint		get_hitpoint_plane(t_ray ray, t_plane plane, bool init_all);
t_plane			get_plane(int offset);

// objects/cylinder.frag
t_hitpoint		get_hitpoint_cylinder(t_ray ray, t_cylinder cylinder, bool init_all);
t_cylinder		get_cylinder(int offset);

// objects/hyperboloid.frag
t_hitpoint		get_hitpoint_hyperboloid(t_ray ray, t_hyperboloid hyperboloid, bool init_all);
t_hyperboloid	get_hyperboloid(int offset);

// point_light.frag
t_point_light	get_point_light(int offset);

// ┌───────────┐
// │ Utilities │
// └───────────┘

// utils/normal_map.frag
vec3			apply_normal_map(t_hitpoint hitpoint);

// utils/ray_utils.frag
vec3			create_bounce_dir(vec3 incoming_dir, vec3 normal);
bool			is_obstructed(t_ray ray);

// utils/hitpoint_utils.frag
vec3			get_color_from_texture(int tex_idx, t_hitpoint hitpoint);
vec3			get_hitpoint_color(t_hitpoint hitpoint);
vec3			get_offset_hitpoint_pos(t_hitpoint hitpoint);
t_hitpoint		get_closest_hitpoint(t_ray ray, bool init_all);
bool			has_image_texture(t_hitpoint hitpoint, out bool texture_is_square);
bool			has_image_texture(t_hitpoint hitpoint);
bool			has_normal_map_material(t_hitpoint hitpoint);

// utils/random.frag
float	rand();
vec3	dither(vec3 col);

// utils/vec3_rotations.frag
vec3	vec3_rotate_x(vec3 v, float rad);
vec3	vec3_rotate_y(vec3 v, float rad);
vec3	vec3_rotate_z(vec3 v, float rad);
vec3	vec3_rotate_axis(vec3 v, vec3 axis, float rad);
