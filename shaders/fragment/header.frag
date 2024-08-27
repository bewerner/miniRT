#DEFINE M_PI 3.1415926535897932384626433832795

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
};

#DEFINE OBJ_NONE		0
#DEFINE OBJ_SPHERE		1
#DEFINE OBJ_PLANE		2
#DEFINE OBJ_CYLINDER	3

struct t_sphere
{
	int				type;
	int				next_offset;
	vec3			origin;
	vec4			base_color;
	float			radius;
};

struct t_plane
{
	int				type;
	int				next_offset;
	vec3			origin;
	vec4			base_color;
	vec3			normal;
	float			dist;
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
	t_plane			cap1;
	t_plane			cap2;
};

#DEFINE LIGHT_NONE		0
#DEFINE LIGHT_POINT		1

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
};



// ┌───────────────────────────┐
// │ in/out and buffer objects │
// └───────────────────────────┘

layout(std140) uniform u_rt
{
	t_rt			rt;
};

in vec4 coord;
uniform samplerBuffer objects;
uniform samplerBuffer lights;

out vec4 FragColor;



// ┌───────────┐
// │ Utilities │
// └───────────┘

// vec3_rotations.frag
vec3	vec3_rotate_x(vec3 v, float rad);
vec3	vec3_rotate_y(vec3 v, float rad);
vec3	vec3_rotate_z(vec3 v, float rad);