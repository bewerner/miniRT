/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_miniRT.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 20:55:35 by bwerner           #+#    #+#             */
/*   Updated: 2024/09/08 19:15:51 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

// static void	init_mlx_images(t_rt *rt)
// {
// 	rt->canvas = mlx_new_image(rt->mlx, rt->mlx->width, rt->mlx->height);
// 	if (!rt->canvas || mlx_image_to_window(rt->mlx, rt->canvas, 0, 0) < 0)
// 		terminate("mlx_new_image failed", 1, rt);
// }

static void	init_glfw(t_rt *rt)
{
	if (glfwInit() == GLFW_FALSE)
		terminate("glfw_init failed", 1, rt);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	rt->window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "miniRT - LOADING...", NULL, NULL);
	if (!rt->window)
		terminate("glfw window creation failed", 1, rt);
	glfwGetWindowSize(rt->window, &rt->width, &rt->height);
	glfwMakeContextCurrent(rt->window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwSwapInterval(SWAP_INTERVAL);

}

// static void	init_mlx(char *filename, t_rt *rt)
// {
// 	t_ivec2	window_size;
// 	t_ivec2	screen_size;
// 	float	ratio;
// 	char	*win_title;

// 	window_size = (t_ivec2){WINDOW_WIDTH, WINDOW_HEIGHT};
// 	win_title = ft_strjoin("miniRT - ", filename);
// 	if (win_title == NULL)
// 		terminate("fatal", 1, rt);
// 	rt->mlx = mlx_init(window_size.x, window_size.y, win_title, 1);
// 	free(win_title);
// 	if (!rt->mlx)
// 		terminate("mlx_init failed", 1, rt);
// 	mlx_get_monitor_size(0, &screen_size.x, &screen_size.y);
// 	ratio = (float)window_size.x / window_size.y;
// 	if (window_size.x > screen_size.x)
// 	{
// 		window_size.x = screen_size.x * 0.9;
// 		window_size.y = (float)window_size.x / ratio;
// 	}
// 	if (window_size.y > screen_size.y)
// 	{
// 		window_size.y = screen_size.y * 0.9;
// 		window_size.x = (float)window_size.y * ratio;
// 	}
// 	mlx_set_window_size(rt->mlx, window_size.x, window_size.y);
// 	mlx_set_window_pos(rt->mlx, ((screen_size.x - rt->mlx->width) / 2),
// 		((screen_size.y - rt->mlx->height) / 2));
// 	init_mlx_images(rt);
// }

static void	init_camera(t_camera *camera)
{
	t_vec3	dir;
	float	rad;

	camera->direction = vec3_normalize(camera->direction);
	dir = camera->direction;
	camera->pitch = atan2f(sqrtf(dir.x * dir.x + dir.y * dir.y), -dir.z);
	camera->yaw = atan2f(dir.x, dir.y) * -1;
	rad = camera->fov * (M_PI / 180);
	camera->focal_length = 1.0f / tanf(rad * 0.5f);
	reset_camera(camera);
}

static void	create_screen_vertices(t_rt *rt)
{
	GLuint	VBO;
	static const float	vertices[] = {
		 1,  1, 0,  1,  0, 0,
		 1, -1, 0,  1,  1, 0,
		-1, -1, 0,  0,  1, 0,

		-1, -1, 0,  0,  1, 0,
		-1,  1, 0,  0,  0, 0,
		 1,  1, 0,  1,  0, 0
	};

	glGenVertexArrays(1, &rt->vertex_array_object);
	glBindVertexArray(rt->vertex_array_object);

	glGenBuffers(1, &VBO);

	// Bind VBO to GL_ARRAY_BUFFER && copy Vertex buffer to GPU
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// LINKING VERTEX ATTRIBUTES
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void	create_ubo_rt(t_rt *rt)
{
	GLuint	blockIndex;

	glGenBuffers(1, &rt->ubo_rt_id);

	glBindBuffer(GL_UNIFORM_BUFFER, rt->ubo_rt_id);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(t_ubo), NULL, GL_STATIC_DRAW);

	blockIndex = glGetUniformBlockIndex(rt->shader_program, "u_rt");
	if (blockIndex == GL_INVALID_INDEX)
		terminate("u_rt not found in shader program", 1, rt);
	glUniformBlockBinding(rt->shader_program, blockIndex, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, rt->ubo_rt_id);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

size_t	get_gpu_objects_size(t_object *object)
{
	size_t	size;

	size = 0;
	while (object)
	{
		if (object->type == OBJ_SPHERE)
			size += sizeof(t_gpu_sphere);
		else if (object->type == OBJ_PLANE)
			size += sizeof(t_gpu_plane);
		else if (object->type == OBJ_CYLINDER)
			size += sizeof(t_gpu_cylinder);
		else if (object->type == OBJ_HYPERBOLOID)
			size += sizeof(t_gpu_hyperboloid);
		object = object->next;
	}
	return (size + sizeof(float));
}

void	init_gpu_sphere(float *buffer, size_t *i, t_sphere *sphere)
{
	t_gpu_sphere	gpu_sphere;
	
	gpu_sphere.type = (float)sphere->type;
	gpu_sphere.next_offset = (float)(*i + sizeof(t_gpu_sphere) / sizeof(float));
	gpu_sphere.origin = sphere->origin;
	gpu_sphere.base_color = sphere->base_color;
	gpu_sphere.radius = sphere->radius;
	gpu_sphere.material_idx = (float)sphere->material->index;
	ft_memmove(&buffer[*i], &gpu_sphere, sizeof(t_gpu_sphere));
	*i = gpu_sphere.next_offset;
}

void	init_gpu_plane(float *buffer, size_t *i, t_plane *plane)
{
	t_gpu_plane	gpu_plane;
	
	gpu_plane.type = (float)plane->type;
	gpu_plane.next_offset = (float)(*i + sizeof(t_gpu_plane) / sizeof(float));
	gpu_plane.origin = plane->origin;
	gpu_plane.base_color = plane->base_color;
	gpu_plane.normal = plane->normal;
	gpu_plane.dist = plane->dist;
	gpu_plane.material_idx = 0;
	if (plane->material != NULL)
		gpu_plane.material_idx = (float)plane->material->index;
	ft_memmove(&buffer[*i], &gpu_plane, sizeof(t_gpu_plane));
	*i = gpu_plane.next_offset;
}

void	init_gpu_cylinder(float *buffer, size_t *i, t_cylinder *cylinder)
{
	t_gpu_cylinder	gpu_cylinder;
	size_t			size_without_caps;
	size_t			i_cap;

	size_without_caps = sizeof(t_gpu_cylinder) - 2 * sizeof(t_gpu_plane);
	gpu_cylinder.type = (float)cylinder->type;
	gpu_cylinder.next_offset = (float)(*i + sizeof(t_gpu_cylinder) / sizeof(float));
	gpu_cylinder.origin = cylinder->origin;
	gpu_cylinder.base_color = cylinder->base_color;
	gpu_cylinder.orientation = cylinder->orientation;
	gpu_cylinder.radius = cylinder->radius;
	gpu_cylinder.height = cylinder->height;
	gpu_cylinder.material_idx = (float)cylinder->material->index;
	ft_memmove(&buffer[*i], &gpu_cylinder, size_without_caps);
	i_cap = *i + size_without_caps / sizeof(float);
	init_gpu_plane(buffer, &i_cap, &cylinder->cap1);
	init_gpu_plane(buffer, &i_cap, &cylinder->cap2);
	*i = gpu_cylinder.next_offset;
}

void	init_gpu_hyperboloid(float *buffer, size_t *i, t_hyperboloid *hyperboloid)
{
	t_gpu_hyperboloid	gpu_hyperboloid;
	size_t				size;

	size = sizeof(t_gpu_hyperboloid);
	gpu_hyperboloid.type = (float)hyperboloid->type;
	gpu_hyperboloid.next_offset = (float)(*i + sizeof(t_gpu_hyperboloid) / sizeof(float));
	gpu_hyperboloid.origin = hyperboloid->origin;
	gpu_hyperboloid.base_color = hyperboloid->base_color;
	gpu_hyperboloid.a = hyperboloid->a;
	gpu_hyperboloid.b = hyperboloid->b;
	gpu_hyperboloid.c = hyperboloid->c;
	gpu_hyperboloid.shape = hyperboloid->shape;
	gpu_hyperboloid.material_idx = (float)hyperboloid->material->index;
	ft_memmove(&buffer[*i], &gpu_hyperboloid, size);
	*i = gpu_hyperboloid.next_offset;
}

void	init_tbo_objects(float *buffer, t_object *object)
{
	size_t i;

	i = 0;
	while (object)
	{
		if (object->type == OBJ_SPHERE)
			init_gpu_sphere(buffer, &i, (t_sphere *)object);
		else if (object->type == OBJ_PLANE)
			init_gpu_plane(buffer, &i, (t_plane *)object);
		else if (object->type == OBJ_CYLINDER)
			init_gpu_cylinder(buffer, &i, (t_cylinder *)object);
		else if (object->type == OBJ_HYPERBOLOID)
			init_gpu_hyperboloid(buffer, &i, (t_hyperboloid *)object);
		object = object->next;
	}
}

void	create_tbo_objects(t_rt *rt)
{
	size_t	size;
	float	*buffer;
	GLuint	texture_id;

	size = get_gpu_objects_size(rt->objects);
	buffer = (float *)ft_calloc(1, size);
	if (!buffer)
		terminate("failed to allocate texture buffer", 1, rt);
	init_tbo_objects(buffer, rt->objects);

	// for(size_t i = 0; i < get_gpu_objects_size(rt->objects) / 4; i++)
	// 	printf("%f\n", buffer[i]);

	glGenBuffers(1, &rt->tbo_objects_id);
	glBindBuffer(GL_TEXTURE_BUFFER, rt->tbo_objects_id);
	glBufferData(GL_TEXTURE_BUFFER, size, buffer, GL_STATIC_DRAW);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);

	free(buffer);

	glGenTextures(1, &texture_id);
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_BUFFER, texture_id);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, rt->tbo_objects_id);

	GLint uniform_location = glGetUniformLocation(rt->shader_program, "objects");
	if (uniform_location == -1)
		terminate("objects not found in shader program", 1, rt);
	glUniform1i(uniform_location, 1);
}

size_t	get_gpu_lights_size(t_light *light)
{
	size_t	size;

	size = 0;
	while (light)
	{
		if (light->type == LIGHT_POINT)
			size += sizeof(t_gpu_point_light);
		light = light->next;
	}
	return (size + sizeof(float));
}

void	init_gpu_point_light(float *buffer, size_t *i, t_point_light *light)
{
	t_gpu_point_light	gpu_light;
	
	gpu_light.type = (float)light->type;
	gpu_light.next_offset = (float)(*i + sizeof(t_gpu_point_light) / sizeof(float));
	gpu_light.origin = light->origin;
	gpu_light.color = light->color;
	gpu_light.power = light->power;
	gpu_light.intensity = light->intensity;
	ft_memmove(&buffer[*i], &gpu_light, sizeof(t_gpu_point_light));
	*i = gpu_light.next_offset;
}

void	init_tbo_lights(float *buffer, t_light *light)
{
	size_t i;

	i = 0;
	while (light)
	{
		if (light->type == LIGHT_POINT)
			init_gpu_point_light(buffer, &i, (t_point_light *)light);
		light = light->next;
	}
}

void	create_tbo_lights(t_rt *rt)
{
	size_t	size;
	float	*buffer;
	GLuint	texture_id;

	size = get_gpu_lights_size(rt->lights);
	buffer = (float *)ft_calloc(1, size);
	if (!buffer)
		terminate("failed to allocate texture buffer", 1, rt);
	init_tbo_lights(buffer, rt->lights);

	// for (size_t i = 0; i < size / 4; i++)
	// {
	// 	printf("%f\n", buffer[i]);
	// }

	glGenBuffers(1, &rt->tbo_lights_id);
	glBindBuffer(GL_TEXTURE_BUFFER, rt->tbo_lights_id);
	glBufferData(GL_TEXTURE_BUFFER, size, buffer, GL_STATIC_DRAW);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);

	free(buffer);

	glGenTextures(1, &texture_id);
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_BUFFER, texture_id);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, rt->tbo_lights_id);

	GLint uniform_location = glGetUniformLocation(rt->shader_program, "lights");
	if (uniform_location == -1)
		terminate("lights not found in shader program", 1, rt);
	glUniform1i(uniform_location, 2);
}

void	init_cursor_is_settable(t_rt *rt)
{
	double	x;
	double	y;
	double	new_x;

	glfwGetCursorPos(rt->window, &x, &y);
	x = (int)x;
	y = (int)y;
	if (x > 0)
		new_x = x - 1;
	else
		new_x = x + 1;
	glfwPollEvents();
	glfwSetCursorPos(rt->window, new_x, y);
	glfwGetCursorPos(rt->window, &x, &y);
	if (x == new_x)
		rt->cursor_is_settable = 1;
	else
		rt->cursor_is_settable = 0;
}

static float	get_next_value(int fd)
{
	char	str[9];
	float	value;

	read(fd, str, 9);
	str[1] = '\0';
	str[8] = '\0';
	value = ft_atoi(str);
	value += (float)ft_atoi(str + 2) / 1000000;
	return (value);
}

void	init_agx_lut_buffer(t_vec3 *buffer, char *filepath, size_t len, t_rt *rt)
{
	int		fd;
	size_t	i;

	fd = open(filepath, O_RDONLY);
	if (fd == -1)
		terminate(filepath, 1, rt);
	i = 0;
	while (i < len)
	{
		buffer[i].x = get_next_value(fd);
		buffer[i].y = get_next_value(fd);
		buffer[i].z = get_next_value(fd);
		i++;
	}
	close(fd);
}

void	create_tbo_agx_lut(char *filepath, t_rt *rt)
{
	static const size_t	resolution = 64 * 64 * 64;
	size_t				size;
	t_vec3				buffer[resolution];
	GLuint				texture_id;
	// GLuint	tbo_agx_lut_id;

	size = resolution * sizeof(t_vec3);
	init_agx_lut_buffer(buffer, filepath, resolution, rt);

	// for (size_t i = 0; i < resolution; i++)
	// 	printf("%f\n", buffer[i]);

	glGenBuffers(1, &rt->tbo_agx_lut_id);
	glBindBuffer(GL_TEXTURE_BUFFER, rt->tbo_agx_lut_id);
	glBufferData(GL_TEXTURE_BUFFER, size, buffer, GL_STATIC_DRAW);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);

	glGenTextures(1, &texture_id);
	glActiveTexture(GL_TEXTURE0 + 3);
	glBindTexture(GL_TEXTURE_BUFFER, texture_id);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, rt->tbo_agx_lut_id);

	GLint uniform_location = glGetUniformLocation(rt->shader_program, "agx_lut");
	if (uniform_location == -1)
		terminate("agx_lut not found in shader program", 1, rt);
	glUniform1i(uniform_location, 3);
}

void	init_mini_rt(char **argv, t_rt *rt)
{
	rt->filename = argv[1];
	load_scene(argv[1], rt);
	init_glfw(rt);
	init_camera(&rt->camera);
	init_cursor_is_settable(rt);
	init_hooks(rt);
	rt->solid_shader_program = create_shader_program("shaders/vertex/screen.vert", "shaders/solid/solid.frag", rt);
	rt->normal_shader_program = create_shader_program("shaders/vertex/screen.vert", "shaders/normal/normal.frag", rt);
	rt->shader_program = create_shader_program("shaders/vertex/screen.vert", "shaders/fragment/raytracer.frag", rt);
	create_screen_vertices(rt);
	create_ubo_rt(rt);
	create_tbo_objects(rt);
	create_tbo_lights(rt);
	create_ubo_materials(rt);
	create_tbo_agx_lut(LUT_PATH, rt);
}
