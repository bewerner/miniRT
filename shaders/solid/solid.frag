#version 330 core

#import ../fragment/header.frag
#import ../fragment/point_light.frag
#import get_solid_diffuse_color.frag
#import get_solid_specular_color.frag
#import light_gizmo.frag
#import ../fragment/sphere.frag
#import ../fragment/plane.frag
#import ../fragment/cylinder.frag
#import trace_ray_solid.frag
#import ../fragment/vec3_rotations.frag
#import ../fragment/random.frag


void main()
{
	vec2 uv = coord.xy;
	uv.x = uv.x * 2 - 1.0;
	uv.y = (uv.y * 2 - 1.0) / rt.aspect_ratio;

	t_ray camera_ray;
	camera_ray.origin = rt.camera.origin;
	vec3 camera_up = cross(rt.camera.direction, rt.camera.right);
	camera_ray.dir = uv.y * camera_up + uv.x * rt.camera.right + rt.camera.focal_length * rt.camera.direction;


	if (is_ligth_gizmo(camera_ray) == true)
	{
		FragColor = VEC4_BLACK;
		return ;
	}
	
	// Dump unused samplerBuffer 
	// Dump here, so that the conditions (is_ligth_gizmo) is executed correctly
	float dump_value_agx_lut = texelFetch(agx_lut, 0).r;

	vec4 col = trace_ray_solid(camera_ray);
	// col = dither(col);

	FragColor = vec4(col.r, col.g, col.b, 1);
}
