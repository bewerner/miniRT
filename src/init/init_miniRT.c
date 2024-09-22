/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_miniRT.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 20:55:35 by bwerner           #+#    #+#             */
/*   Updated: 2024/09/23 00:24:07 by bwerner          ###   ########.fr       */
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
	glfwInitHint(GLFW_COCOA_CHDIR_RESOURCES, GLFW_FALSE);
	if (glfwInit() == GLFW_FALSE)
		terminate("glfw_init failed", NULL, 1, rt);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	rt->window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "miniRT - LOADING...", NULL, NULL);
	if (!rt->window)
		terminate("glfw window creation failed", NULL, 1, rt);
	glfwMakeContextCurrent(rt->window);
	glfwShowWindow(rt->window);
	glfwPollEvents();
	glfwGetFramebufferSize(rt->window, &rt->width, &rt->height);
	rt->aspect_ratio = (float)rt->width / (float)rt->height;
	glfwGetWindowContentScale(rt->window, &rt->dpi_scale, NULL);

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
		 1,  1, 0,  1, 0, 0,
		 1, -1, 0,  1, 1, 0,
		-1, -1, 0,  0, 1, 0,

		-1, -1, 0,  0, 1, 0,
		-1,  1, 0,  0, 0, 0,
		 1,  1, 0,  1, 0, 0
	};

	glGenVertexArrays(1, &rt->vao_screen_id);
	glBindVertexArray(rt->vao_screen_id);

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

static void	create_gizmo(t_rt *rt)
{
	GLuint	VBO;
	static const float	vertices[] = {
		-127,  127, 200,		-1,-1, 0.5,
		 127,  127, 200,		 1,-1, 0.5,
		-127, -127, 200,		-1, 1, 0.5,

		 127,  127, 200,		 1,-1, 0.5,
		 127, -127, 200,		 1, 1, 0.5,
		-127, -127, 200,		-1, 1, 0.5,



		
		  0,  3.31, 0,		1, 0, 0,
		100,  3.31, 0,		1, 0, 0,
		  0, -1.6, 0,		1, 0, 0,

		100,  3.31, 0,		1, 0, 0,
		100, -3.31, 0,		1, 0, 0,
		  0, -3.31, 0,		1, 0, 0,

		  0, 0, -3.31,		1, 0, 0,
		100, 0, -3.31,		1, 0, 0,
		  0, 0,  3.31,		1, 0, 0,

		100, 0, -3.31,		1, 0, 0,
		100, 0,  3.31,		1, 0, 0,
		  0, 0,  3.31,		1, 0, 0,

		  3.31,   0, 0,		0, 1, 0,
		  3.31, 100, 0,		0, 1, 0,
		 -3.31,   0, 0,		0, 1, 0,

		  3.31, 100, 0,		0, 1, 0,
		 -3.31, 100, 0,		0, 1, 0,
		 -3.31,   0, 0,		0, 1, 0,

		  0,   0,-3.31,		0, 1, 0,
		  0, 100,-3.31,		0, 1, 0,
		  0,   0, 3.31,		0, 1, 0,

		  0, 100,-3.31,		0, 1, 0,
		  0, 100, 3.31,		0, 1, 0,
		  0,   0, 3.31,		0, 1, 0,

		  3.31, 0,   0,		0, 0, 1,
		  3.31, 0,-100,		0, 0, 1,
		 -3.31, 0,   0,		0, 0, 1,

		  3.31, 0,-100,		0, 0, 1,
		 -3.31, 0,-100,		0, 0, 1,
		 -3.31, 0,   0,		0, 0, 1,



		   -27, 0,  27,		-1,-1, 2,
		    27, 0,  27,		 1,-1, 2,
		   -27, 0, -27,		-1, 1, 2,

		    27, 0,  27,		 1,-1, 2,
		    27, 0, -27,		 1, 1, 2,
		   -27, 0, -27,		-1, 1, 2,

		   -27, 0,  27,		-1,-1, 3,
		    27, 0,  27,		 1,-1, 3,
		   -27, 0, -27,		-1, 1, 3,

		    27, 0,  27,		 1,-1, 3,
		    27, 0, -27,		 1, 1, 3,
		   -27, 0, -27,		-1, 1, 3,

		   -27, 0,  27,		-1,-1, 4,
		    27, 0,  27,		 1,-1, 4,
		   -27, 0, -27,		-1, 1, 4,

		    27, 0,  27,		 1,-1, 4,
		    27, 0, -27,		 1, 1, 4,
		   -27, 0, -27,		-1, 1, 4,



		   -27, 0,  27,		-1,-1, -2,
		    27, 0,  27,		 1,-1, -2,
		   -27, 0, -27,		-1, 1, -2,

		    27, 0,  27,		 1,-1, -2,
		    27, 0, -27,		 1, 1, -2,
		   -27, 0, -27,		-1, 1, -2,

		   -27, 0,  27,		-1,-1, -3,
		    27, 0,  27,		 1,-1, -3,
		   -27, 0, -27,		-1, 1, -3,

		    27, 0,  27,		 1,-1, -3,
		    27, 0, -27,		 1, 1, -3,
		   -27, 0, -27,		-1, 1, -3,

		   -27, 0,  27,		-1,-1, -4,
		    27, 0,  27,		 1,-1, -4,
		   -27, 0, -27,		-1, 1, -4,

		    27, 0,  27,		 1,-1, -4,
		    27, 0, -27,		 1, 1, -4,
		   -27, 0, -27,		-1, 1, -4
	};

	glGenVertexArrays(1, &rt->vao_gizmo_id);
	glBindVertexArray(rt->vao_gizmo_id);

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

	blockIndex = glGetUniformBlockIndex(rt->preview_shader_program, "u_rt");
	if (blockIndex == GL_INVALID_INDEX)
		terminate("u_rt not found in shader program", NULL, 1, rt);
	glUniformBlockBinding(rt->preview_shader_program, blockIndex, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, rt->ubo_rt_id);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
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

void	create_fbo(t_rt *rt)
{
	// Create texture for the framebuffer
	glGenTextures(1, &rt->tex_fbo_id);
	glBindTexture(GL_TEXTURE_2D, rt->tex_fbo_id);
	glfwGetFramebufferSize(rt->window, &rt->width, &rt->height);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, rt->width, rt->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Create framebuffer and attach the texture
	glGenFramebuffers(1, &rt->fbo_id);
	glBindFramebuffer(GL_FRAMEBUFFER, rt->fbo_id);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rt->tex_fbo_id, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		terminate("Framebuffer is not complete!\n", NULL, 1, rt);

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind the framebuffer
}

void	create_environment_map(char * filepath, float strength, t_rt *rt)
{
	int		width;
	int		height;
	t_vec3	*image;
	size_t	i;

	image = (t_vec3 *)stbi_loadf(filepath, &width, &height, NULL, 3);
	if (!image)
		terminate(filepath, NULL, 1, rt);
	i = 0;
	while (i < (size_t)width * height)
	{
		image[i] = vec3_scale(strength, image[i]);
		i++;
	}
	glGenTextures(1, &rt->environment_map_id);
	glActiveTexture(GL_TEXTURE0 + 4);
	glBindTexture(GL_TEXTURE_2D, rt->environment_map_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0,
		GL_RGB, GL_FLOAT, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(image);
}

void	init_shader_programs(t_rt *rt)
{
	rt->solid_shader_program = create_shader_program(
			"shaders/vertex/screen.vert",
			"shaders/solid/solid.frag", rt);
	rt->normal_shader_program = create_shader_program(
			"shaders/vertex/screen.vert",
			"shaders/normal/normal.frag", rt);
	rt->postprocessing_shader_program = create_shader_program(
			"shaders/vertex/screen.vert",
			"shaders/postprocessing/postprocessing.frag", rt);
	rt->preview_shader_program = create_shader_program(
			"shaders/vertex/screen.vert",
			"shaders/fragment/raytracer.frag", rt);
	rt->gizmo_shader_program = create_shader_program(
			"shaders/vertex/gizmo.vert",
			"shaders/gizmo/gizmo.frag", rt);
}

void	init_mini_rt(char **argv, t_rt *rt)
{
	rt->max_samples = INT32_MAX - 1;
	// rt->max_samples = 4096;
	rt->filename = argv[1];
	load_scene(argv[1], rt);
	init_glfw(rt);
	init_cursor_is_settable(rt);
	init_camera(&rt->camera);
	init_hooks(rt);
	init_shader_programs(rt);
	create_screen_vertices(rt);
	create_gizmo(rt);
	create_ubo_rt(rt);
	create_tbo_objects(rt);
	create_tbo_lights(rt);
	create_ubo_materials(rt);
	create_tbo_agx_lut(LUT_PATH, rt);
	create_fbo(rt);
	create_environment_map("assets/syferfontein_18d_clear_2k.hdr", 1.0f, rt);
}
