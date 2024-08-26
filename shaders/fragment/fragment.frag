#version 330 core

out vec4 FragColor;

in vec4 coord;

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

struct t_sphere
{
	vec3	origin;
	float	radius;
};

struct t_light
{
	vec3	origin;
	vec4	col;
	float	power;
	float	intensity;
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

layout(std140) uniform u_rt
{
	t_rt			rt;
};

uniform samplerBuffer objects;
uniform samplerBuffer lights;


vec3	vec3_rotate_x(vec3 v, float rad);
vec3	vec3_rotate_y(vec3 v, float rad);
vec3	vec3_rotate_z(vec3 v, float rad);








float	get_discriminant(t_ray ray, t_sphere sphere, out float t0, out float t1)
{
	vec3		ap;
	float		a;
	float		b;
	float		discriminant;
	float		sqrt_discriminant;

	ap = ray.origin - sphere.origin;
	a = dot(ray.dir, ray.dir);
	b = 2 * dot(ap, ray.dir);
	discriminant = b * b - 4 * a * (dot(ap, ap) - sphere.radius * sphere.radius);
	if (discriminant < 0)
		return (discriminant);
	sqrt_discriminant = sqrt(discriminant);
	t0 = (-b + sqrt_discriminant) / (2 * a);
	t1 = (-b - sqrt_discriminant) / (2 * a);
	return (discriminant);
}

t_hitpoint	get_hitpoint_sphere(t_ray ray, t_sphere sphere)
{
	t_hitpoint	hitpoint;
	float		discriminant;
	float		t0;
	float		t1;

	discriminant = get_discriminant(ray, sphere, t0, t1);
	if (discriminant < 0 || (t1 < 0 && t0 < 0))
		return (t_hitpoint(false, vec3(1.0 / 0.0, 1.0 / 0.0, 1.0 / 0.0), vec3(1.0 / 0.0, 1.0 / 0.0, 1.0 / 0.0), vec3(1.0 / 0.0, 1.0 / 0.0, 1.0 / 0.0)));
	if (t1 < 0)
	{
		hitpoint.hit = true;
		hitpoint.ray = t0 * ray.dir;
		hitpoint.pos = ray.origin + hitpoint.ray;
		hitpoint.normal = normalize(sphere.origin - hitpoint.pos);
	}
	else
	{
		hitpoint.hit = true;
		hitpoint.ray = t1 * ray.dir;
		hitpoint.pos = ray.origin + hitpoint.ray;
		hitpoint.normal	= normalize(hitpoint.pos - sphere.origin);
	}
	return (hitpoint);
}

bool	is_obstructed(t_ray ray)
{
	t_hitpoint	current;
	float		ray_len;

	// current.ray = g_vec3_inf;
	// current.pos = g_vec3_inf;
	// current.object = NULL;
	// ray_len = vec3_len(ray.dir);
	// object = rt->objects;
	// while (object)
	// {
	// 	if (object->type == OBJ_SPHERE)
	// 		current = get_hitpoint_sphere(ray, (t_sphere *)object);
	// 	else if (object->type == OBJ_PLANE)
	// 		current = get_hitpoint_plane(ray, (t_plane *)object);
	// 	else if (object->type == OBJ_CYLINDER)
	// 		current = get_hitpoint_cylinder(ray, (t_cylinder *)object);
	// 	if (vec3_len(current.ray) < ray_len)
	// 		return (true);
	// 	object = object->next;
	// }
	return (false);
}

vec4	illuminate_from_point_light(t_light light, t_hitpoint hitpoint)
{
	t_ray	light_ray;
	float	distance;
	float	intensity;

	light_ray.dir = hitpoint.pos + 0.001 * hitpoint.normal - light.origin;
	light_ray.origin = light.origin;
	intensity = -dot(hitpoint.normal, normalize(light_ray.dir));
	if (intensity <= 0)
		return (vec4(0,0,0,0));
	distance = length(light_ray.dir);
	intensity = intensity / (distance * distance);
	if (intensity <= 0.000005 || is_obstructed(light_ray))
		return (vec4(0,0,0,0));
	intensity *= light.intensity;
	return (intensity * light.col);
}


vec4	get_diffuse_color(t_hitpoint hitpoint)
{
	t_light	light;
	vec4	final_col;
	vec4	light_col;

	final_col = vec4(0,0,0,0);

	light.origin	= vec3(texelFetch(lights, 2).r, texelFetch(lights, 3).r, texelFetch(lights, 4).r);
	light.col		= vec4(texelFetch(lights, 5).r,texelFetch(lights, 6).r,texelFetch(lights, 7).r,texelFetch(lights, 8).r);
	light.power		= texelFetch(lights, 10).r;
	// light.origin	= vec3(-3,-3,-3);
	// light.col		= vec4(1,1,1,1);
	// light.power		= 325.0;
	light.intensity = light.power / 22.86;

	light_col = illuminate_from_point_light(light, hitpoint);
	final_col = final_col + light_col;

	final_col = vec4(1,0,0,1) * final_col;
	final_col = final_col + rt.ambient;
	return (final_col);
}


vec4	get_normal_color(t_hitpoint hitpoint)
{
	return (vec4(vec3(hitpoint.normal.xyz * 0.5 + 0.5), 1.0f));
}

void main()
{

	vec2 uv = coord.xy;

	uv.x = uv.x * 2 - 1.0;
	// uv.y = (uv.y * 2 - 1.0) / (1280.0 / 720.0);
	uv.y = (uv.y * 2 - 1.0) / rt.aspect_ratio;

	vec4 col;
	col = coord;
	col = vec4(0,0,0,1);

	float M_PI = 3.1415926535897932384626433832795;
	// float focal_len = 0.5 / tan(95 * M_PI / 180.0 * 0.5);

	// t_sphere sp = t_sphere(vec3(0.0, 0.0, 0), 1.0);
	// t_sphere sp = t_sphere(vec3(2, 2, 2), 1.0);
	t_sphere sp = t_sphere(vec3(texelFetch(objects, 2).r, texelFetch(objects, 3).r, texelFetch(objects, 4).r), texelFetch(objects, 8).r);

	t_ray camera_ray;
	// camera_ray.origin = vec3(uv.xy, -3.0);
	// camera_ray.dir = vec3(uv.xy, focal_len);

	camera_ray.origin = rt.camera.origin;
	// camera_ray.dir = vec3(uv.xy, -rt.camera.focal_length);
	vec3 camera_up = cross(rt.camera.direction, rt.camera.right);
	camera_ray.dir = uv.y * camera_up + uv.x * rt.camera.right + rt.camera.focal_length * rt.camera.direction;
	

	// camera_ray.dir = vec3_rotate_x(camera_ray.dir, rt.camera.pitch);
	// camera_ray.dir = vec3_rotate_z(camera_ray.dir, rt.camera.yaw);

	t_hitpoint hitpoint = get_hitpoint_sphere(camera_ray, sp);
	if (hitpoint.hit == true)
		col = get_diffuse_color(hitpoint);
		// col = get_normal_color(hitpoint);

	FragColor = col;
	// if (texelFetch(lights, 0).r == 1)
	// 	FragColor = vec4(0.2,0,0,1);
}
