/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_miniRT.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 20:55:35 by bwerner           #+#    #+#             */
/*   Updated: 2024/12/21 23:40:56 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

static void	init_camera(t_camera *camera, t_rt *rt)
{
	t_vec3	dir;
	float	rad;

	camera->direction = vec3_normalize(camera->direction);
	dir = camera->direction;
	camera->pitch = atan2f(sqrtf(dir.x * dir.x + dir.y * dir.y), -dir.z);
	camera->yaw = atan2f(dir.x, dir.y) * -1;
	// printf("Camera rotation in blender: X %f, Z %f\n", camera->pitch * (180.0f/M_PI), camera->yaw * (180.0f/M_PI));
	rad = camera->fov * (M_PI / 180);
	camera->focal_length = 1.0f / tanf(rad * 0.5f);
	reset_camera(camera, rt);
}

void	create_ubo_rt(t_rt *rt)
{
	GLuint	block_index;

	glGenBuffers(1, &rt->ubo_rt_id);
	glBindBuffer(GL_UNIFORM_BUFFER, rt->ubo_rt_id);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(t_ubo), NULL, GL_STATIC_DRAW);
	block_index = glGetUniformBlockIndex(rt->render_shader_program, "u_rt");
	if (block_index == GL_INVALID_INDEX)
		terminate("u_rt not found in shader program", NULL, 1, rt);
	glUniformBlockBinding(rt->render_shader_program, block_index, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, rt->ubo_rt_id);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void	create_fbo(t_rt *rt, float render_scale)
{
	glGenTextures(1, &rt->tex_fbo_id);
	glBindTexture(GL_TEXTURE_2D_ARRAY, rt->tex_fbo_id);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA32F, (rt->width * render_scale), (rt->height * render_scale), 8, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	glGenFramebuffers(1, &rt->fbo_id);
	glBindFramebuffer(GL_FRAMEBUFFER, rt->fbo_id);
	for (int i = 0; i < 8; i++)
		glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, rt->tex_fbo_id, 0, i);
	set_drawbuffers(rt);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		terminate("Framebuffer creation failed!\n", NULL, 1, rt);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void	init_mini_rt(char **argv, t_rt *rt)
{
	rt->cursor_is_settable = !WSL;
	rt->filename = argv[1];
	load_scene(argv[1], rt);
	init_glfw(rt);
	init_camera(&rt->camera, rt);
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
	rt->max_render_scale = 1;
	rt->render_scale = 1;
	create_fbo(rt, rt->render_scale);
	create_environment_map(rt);
	load_textures(rt);
}
