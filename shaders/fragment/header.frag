#define M_PI		3.1415926535897932384626433832795
#define EPSILON		0.0001
#define INF			(1.0 / 0.0)
#define VEC2_INF	vec2(INF, INF)
#define VEC3_INF	vec3(INF, INF, INF)
#define VEC3_BLACK	vec3(0, 0, 0)
#define VEC3_WHITE	vec3(1, 1, 1)
#define VEC4_BLACK	vec4(0, 0, 0, 1)
#define HP_INF		t_hitpoint(false, VEC3_INF, VEC3_INF, VEC3_INF, VEC2_INF, VEC3_BLACK, 0)

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
	float		padding1;
};

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

in vec2 coord;
uniform samplerBuffer objects;
uniform samplerBuffer lights;
uniform samplerBuffer agx_lut;

out vec3 FragColor;

// AgX.frag
vec3			to_agx(vec3 col);

// checker_texture.frag
vec3			get_checker_color(t_hitpoint hitpoint);

// get_illumination_color.frag
vec3			get_illumination_color(t_hitpoint hitpoint);

// get_sky_color.frag
vec3			get_sky_color(t_hitpoint hitpoint);

// get_diffuse_color.frag
vec3			get_diffuse_color(t_hitpoint hitpoint);

// get_reflection_color.frag
vec3			get_reflection_color(t_hitpoint hp);

// get_specualar_color.frag
vec3			get_specular_color(t_hitpoint hitpoint, vec3 col_illumination);

// trace_ray.frag
vec3			trace_ray(t_ray ray);

// ┌─────────┐
// │ Objects │
// └─────────┘

// objects/object_utils.frag
int				next_object_type(inout int i);
int				next_light_type(inout int i);

// objects/sphere.frag
t_hitpoint		get_hitpoint_sphere(t_ray ray, t_sphere sphere);
t_sphere		get_sphere(int offset);

// objects/plane.frag
bool			is_near_zero(float value);
t_hitpoint		get_hitpoint_plane(t_ray ray, t_plane plane);
t_plane			get_plane(int offset);

// objects/cylinder.frag
t_hitpoint		get_hitpoint_cylinder(t_ray ray, t_cylinder cylinder);
t_cylinder		get_cylinder(int offset);

// objects/hyperboloid.frag
t_hitpoint		get_hitpoint_hyperboloid(t_ray ray, t_hyperboloid hyperboloid);
t_hyperboloid	get_hyperboloid(int offset);

// point_light.frag
t_point_light	get_point_light(int offset);

// ┌───────────┐
// │ Utilities │
// └───────────┘

// utils/ray_utils.frag
vec3			create_bounce_dir(vec3 incoming_dir, vec3 normal);
bool			is_obstructed(t_ray ray);

// utils/hitpoint_utils.frag
vec3			get_offset_hitpoint_pos(t_hitpoint hitpoint);
t_hitpoint		get_closest_hitpoint(t_ray ray);

// utils/random.frag
float	rand();
vec3	dither(vec3 col);

// utils/vec3_rotations.frag
vec3	vec3_rotate_x(vec3 v, float rad);
vec3	vec3_rotate_y(vec3 v, float rad);
vec3	vec3_rotate_z(vec3 v, float rad);
