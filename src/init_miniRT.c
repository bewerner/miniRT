/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_miniRT.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 20:55:35 by bwerner           #+#    #+#             */
/*   Updated: 2024/08/26 17:26:48 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

// static void	init_mlx_images(t_rt *rt)
// {
// 	rt->canvas = mlx_new_image(rt->mlx, rt->mlx->width, rt->mlx->height);
// 	if (!rt->canvas || mlx_image_to_window(rt->mlx, rt->canvas, 0, 0) < 0)
// 		terminate("mlx_new_image failed", 1, rt);
// }

static void	init_glfw(char *filename, t_rt *rt)
{
	char	*win_title;

	if (glfwInit() == GLFW_FALSE)
		terminate("glfw_init failed", 1, rt);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	win_title = ft_strjoin("miniRT - ", filename);
	if (win_title == NULL)
		terminate("fatal", 1, rt);
	rt->window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, win_title, NULL, NULL);
	free(win_title);
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
	camera->focal_length = 0.5f / tanf(rad * 0.5f);
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

void	create_uniform_buffer_rt(t_rt *rt)
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

void	init_mini_rt(char **argv, t_rt *rt)
{
	load_scene(argv[1], rt);
	init_glfw(argv[1], rt);
	init_camera(&rt->camera);
	init_hooks(rt);
	create_shader_program(rt);
	create_screen_vertices(rt);
	create_uniform_buffer_rt(rt);
}
