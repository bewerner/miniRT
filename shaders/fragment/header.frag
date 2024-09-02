#define M_PI		3.1415926535897932384626433832795
#define EPSILON		0.0001
#define INF			(1.0 / 0.0)
#define VEC3_INF	vec3(INF, INF, INF)
#define VEC4_BLACK	vec4(0, 0, 0, 1)
#define HP_INF		t_hitpoint(false, VEC3_INF, VEC3_INF, VEC3_INF, VEC4_BLACK, 0)

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
	vec4			color;
	int				material_idx;
};

#define OBJ_NONE		0
#define OBJ_SPHERE		1
#define OBJ_PLANE		2
#define OBJ_CYLINDER	3

struct t_sphere
{
	int				type;
	int				next_offset;
	vec3			origin;
	vec4			base_color;
	float			radius;
	int				material_idx;
};

struct t_plane
{
	int				type;
	int				next_offset;
	vec3			origin;
	vec4			base_color;
	vec3			normal;
	float			dist;
	int				material_idx;
};

struct t_cylinder
{
	int				type;
	int				next_offset;
	vec3			origin;
	vec4			base_color;
	vec3			orientation;
	float			radius;
	float			height;
	int				material_idx;
	t_plane			cap1;
	t_plane			cap2;
};

#define LIGHT_NONE		0
#define LIGHT_POINT		1

struct t_point_light
{
	int				type;
	int				next_offset;
	vec3			origin;
	vec4			color;
	float			ratio;
	float			power;
	float			intensity;
};

struct t_material
{
	vec4		color;
	float		metallic;
	float		roughness;
	float		ior;
	float		transmission;
	vec4		emission_color;
	float		emission_strength;
	float		padding1;
	float		padding2;
	float		padding3;
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
	vec4			ambient;
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

in vec4 coord;
uniform samplerBuffer objects;
uniform samplerBuffer lights;
uniform samplerBuffer agx_lut;

out vec4 FragColor;

// point_light.frag
t_point_light	get_point_light(int offset);

// get_diffuse_color.frag
vec4			illuminate_from_point_light(t_point_light point_light, t_hitpoint hitpoint);
vec4			get_diffuse_color(t_hitpoint hitpoint);

// get_reflection_color.frag
vec4			get_reflection_color(t_hitpoint hp);

// get_specualar_color.frag
vec4			get_specular_color(t_hitpoint hitpoint);

// sphere.frag
t_hitpoint		get_hitpoint_sphere(t_ray ray, t_sphere sphere);
t_sphere		get_sphere(int offset);

// plane.frag
t_hitpoint		get_hitpoint_plane(t_ray ray, t_plane plane);
t_plane			get_plane(int offset);

// cylinder.frag
t_hitpoint		get_hitpoint_cylinder(t_ray ray, t_cylinder cylinder);
t_cylinder		get_cylinder(int offset);


// trace_ray.frag
t_hitpoint		get_closest_hitpoint(t_ray ray);
vec3			create_bounce_dir(vec3 incoming_dir, vec3 normal);
bool			is_obstructed(t_ray ray);
vec4			trace_ray(t_ray ray);

// ┌───────────┐
// │ Utilities │
// └───────────┘

// random.frag
float	rand();
vec4	dither(vec4 col);

// vec3_rotations.frag
vec3	vec3_rotate_x(vec3 v, float rad);
vec3	vec3_rotate_y(vec3 v, float rad);
vec3	vec3_rotate_z(vec3 v, float rad);

// float	g_metallic = 0.5;
