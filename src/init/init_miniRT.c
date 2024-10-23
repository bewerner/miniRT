/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_miniRT.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 20:55:35 by bwerner           #+#    #+#             */
/*   Updated: 2024/10/23 17:43:44 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

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

void	create_ubo_rt(t_rt *rt)
{
	GLuint	block_index;

	glGenBuffers(1, &rt->ubo_rt_id);
	glBindBuffer(GL_UNIFORM_BUFFER, rt->ubo_rt_id);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(t_ubo), NULL, GL_STATIC_DRAW);
	block_index = glGetUniformBlockIndex(rt->preview_shader_program, "u_rt");
	if (block_index == GL_INVALID_INDEX)
		terminate("u_rt not found in shader program", NULL, 1, rt);
	glUniformBlockBinding(rt->preview_shader_program, block_index, 0);
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
	int	i;

	glGenTextures(1, &rt->tex_fbo_id);
	glBindTexture(GL_TEXTURE_2D_ARRAY, rt->tex_fbo_id);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA32F, rt->width, rt->height,
		8, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	glGenFramebuffers(1, &rt->fbo_id);
	glBindFramebuffer(GL_FRAMEBUFFER, rt->fbo_id);
	i = 0;
	while (i < 8)
	{
		glFramebufferTextureLayer(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, rt->tex_fbo_id, 0, i);
		i++;
	}
	set_drawbuffers(rt);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		terminate("Framebuffer creation failed!\n", NULL, 1, rt);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void	init_mini_rt(char **argv, t_rt *rt)
{
	rt->filename = argv[1];
	load_scene(argv[1], rt);
	init_glfw(rt);
	init_cursor_is_settable(rt);
	init_camera(&rt->camera);
	init_hooks(rt);
	init_shader_programs(rt);
	create_vbo_screen_vertices(rt);
	create_vbo_gizmo(rt);
	create_ubo_rt(rt);
	create_tbo_objects(rt);
	create_tbo_lights(rt);
	create_ubo_textures(rt);
	create_ubo_materials(rt);
	create_tbo_agx_lut(LUT_PATH, rt);
	create_fbo(rt);
	create_environment_map(rt);
	load_textures(rt);
}
