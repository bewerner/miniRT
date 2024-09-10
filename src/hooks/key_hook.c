/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hook.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 19:55:23 by bwerner           #+#    #+#             */
/*   Updated: 2024/09/10 20:04:11 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

void	reset_camera(t_camera *camera)
{
	static t_camera	cam;
	static bool		is_set;

	if (is_set == false)
	{
		cam = *camera;
		is_set = true;
	}
	else
		*camera = cam;
}

static void	key_hook_axial_view(int key, int action, t_rt *rt)
{
	if (key == GLFW_KEY_KP_1 && action == GLFW_PRESS)
	{
		rt->camera.origin = (t_vec3){{0, -vec3_len(rt->camera.origin), 0}};
		rt->camera.yaw = 0;
		rt->camera.pitch = M_PI / 2;
		rt->move.vel = g_vec3_zero;
	}
	else if (key == GLFW_KEY_KP_3 && action == GLFW_PRESS)
	{
		rt->camera.origin = (t_vec3){{-vec3_len(rt->camera.origin), 0, 0}};
		rt->camera.yaw = -M_PI / 2;
		rt->camera.pitch = M_PI / 2;
		rt->move.vel = g_vec3_zero;
	}
	else if (key == GLFW_KEY_KP_7 && action == GLFW_PRESS)
	{
		rt->camera.origin = (t_vec3){{0, 0, vec3_len(rt->camera.origin)}};
		rt->camera.yaw = 0;
		rt->camera.pitch = 0;
		rt->move.vel = g_vec3_zero;
	}
}

void	key_hook(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	t_rt	*rt;

	(void)scancode;
	rt = get_rt();
	if (((key == GLFW_KEY_ESCAPE) || (key == GLFW_KEY_Q))
		&& action == GLFW_PRESS && !mods)
		glfwSetWindowShouldClose(window, true);
	else if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
	{
		rt->mode++;
		if (rt->mode > MODE_PREVIEW)
			rt->mode = MODE_SOLID;

		if (rt->mode == MODE_SOLID)
		{
			glUseProgram(rt->solid_shader_program);

			glBindBufferBase(GL_UNIFORM_BUFFER, 0, rt->ubo_rt_id);
			glBindBufferBase(GL_UNIFORM_BUFFER, 0, 0); // unbind
			
			glBindBuffer(GL_TEXTURE_BUFFER, rt->tbo_objects_id);
			glActiveTexture(GL_TEXTURE0 + 1);
			// glBindTexture(GL_TEXTURE_BUFFER, rt->objects_texture_id);
			glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, rt->tbo_objects_id);
			GLint uniform_location = glGetUniformLocation(rt->shader_program, "objects");
			if (uniform_location == -1)
				terminate("objects not found in shader program", 1, rt);
			glUniform1i(uniform_location, 1);
			glBindBuffer(GL_TEXTURE_BUFFER, 0);	// unbind

			glBindBuffer(GL_TEXTURE_BUFFER, rt->tbo_lights_id);
			glActiveTexture(GL_TEXTURE0 + 2);
			// glBindTexture(GL_TEXTURE_BUFFER, rt->lights_texture_id);
			glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, rt->tbo_lights_id);
			uniform_location = glGetUniformLocation(rt->shader_program, "lights");
			if (uniform_location == -1)
				terminate("lights not found in shader program", 1, rt);
			glUniform1i(uniform_location, 2);
			glBindBuffer(GL_TEXTURE_BUFFER, 0);	// unbind

			// glBindBuffer(GL_TEXTURE_BUFFER, rt->tbo_materials_id);
			// glActiveTexture(GL_TEXTURE0 + 3);
			// // glBindTexture(GL_TEXTURE_BUFFER, rt->materials_texture_id);
			// glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, rt->tbo_lights_id);
			// uniform_location = glGetUniformLocation(rt->shader_program, "materials");
			// if (uniform_location == -1)
			// 	terminate("lights not found in shader program", 1, rt);
			// glUniform1i(uniform_location, 3);
			// glBindBuffer(GL_TEXTURE_BUFFER, 0);	// unbind

			// RE-BIND AGX
			glBindBuffer(GL_TEXTURE_BUFFER, rt->tbo_agx_lut_id);
			glActiveTexture(GL_TEXTURE0 + 3);
			glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, rt->tbo_agx_lut_id);
			uniform_location = glGetUniformLocation(rt->shader_program, "agx_lut");
			if (uniform_location == -1)
				terminate("agx_lut not found in shader program", 1, rt);
			glUniform1i(uniform_location, 3);
			glBindBuffer(GL_TEXTURE_BUFFER, 0);	// unbind

		}
		if (rt->mode == MODE_NORMAL)
		{
			glUseProgram(rt->normal_shader_program);

			glBindBufferBase(GL_UNIFORM_BUFFER, 0, rt->ubo_rt_id);
			glBindBufferBase(GL_UNIFORM_BUFFER, 0, 0); // unbind
			
			glBindBuffer(GL_TEXTURE_BUFFER, rt->tbo_objects_id);
			glActiveTexture(GL_TEXTURE0 + 1);
			// glBindTexture(GL_TEXTURE_BUFFER, rt->objects_texture_id);
			glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, rt->tbo_objects_id);
			GLint uniform_location = glGetUniformLocation(rt->shader_program, "objects");
			if (uniform_location == -1)
				terminate("objects not found in shader program", 1, rt);
			glUniform1i(uniform_location, 1);
			glBindBuffer(GL_TEXTURE_BUFFER, 0);	// unbind

			glBindBuffer(GL_TEXTURE_BUFFER, rt->tbo_lights_id);
			glActiveTexture(GL_TEXTURE0 + 2);
			// glBindTexture(GL_TEXTURE_BUFFER, rt->lights_texture_id);
			glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, rt->tbo_lights_id);
			uniform_location = glGetUniformLocation(rt->shader_program, "lights");
			if (uniform_location == -1)
				terminate("lights not found in shader program", 1, rt);
			glUniform1i(uniform_location, 2);
			glBindBuffer(GL_TEXTURE_BUFFER, 0);	// unbind

			// glBindBuffer(GL_TEXTURE_BUFFER, rt->tbo_materials_id);
			// glActiveTexture(GL_TEXTURE0 + 3);
			// // glBindTexture(GL_TEXTURE_BUFFER, rt->materials_texture_id);
			// glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, rt->tbo_lights_id);
			// uniform_location = glGetUniformLocation(rt->shader_program, "materials");
			// if (uniform_location == -1)
			// 	terminate("lights not found in shader program", 1, rt);
			// glUniform1i(uniform_location, 3);
			// glBindBuffer(GL_TEXTURE_BUFFER, 0);	// unbind

			// RE-BIND AGX
			glBindBuffer(GL_TEXTURE_BUFFER, rt->tbo_agx_lut_id);
			glActiveTexture(GL_TEXTURE0 + 3);
			glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, rt->tbo_agx_lut_id);
			uniform_location = glGetUniformLocation(rt->shader_program, "agx_lut");
			if (uniform_location == -1)
				terminate("agx_lut not found in shader program", 1, rt);
			glUniform1i(uniform_location, 3);
			glBindBuffer(GL_TEXTURE_BUFFER, 0);	// unbind

		}
		else if (rt->mode == MODE_PREVIEW)
		{
			glUseProgram(rt->shader_program);

			glBindBufferBase(GL_UNIFORM_BUFFER, 0, rt->ubo_rt_id);
			glBindBufferBase(GL_UNIFORM_BUFFER, 0, 0); // unbind
			
			glBindBuffer(GL_TEXTURE_BUFFER, rt->tbo_objects_id);
			glActiveTexture(GL_TEXTURE0 + 1);
			// glBindTexture(GL_TEXTURE_BUFFER, rt->objects_texture_id);
			glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, rt->tbo_objects_id);
			GLint uniform_location = glGetUniformLocation(rt->shader_program, "objects");
			if (uniform_location == -1)
				terminate("objects not found in shader program", 1, rt);
			glUniform1i(uniform_location, 1);
			glBindBuffer(GL_TEXTURE_BUFFER, 0);	// unbind

			glBindBuffer(GL_TEXTURE_BUFFER, rt->tbo_lights_id);
			glActiveTexture(GL_TEXTURE0 + 2);
			// glBindTexture(GL_TEXTURE_BUFFER, rt->lights_texture_id);
			glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, rt->tbo_lights_id);
			uniform_location = glGetUniformLocation(rt->shader_program, "lights");
			if (uniform_location == -1)
				terminate("lights not found in shader program", 1, rt);
			glUniform1i(uniform_location, 2);
			glBindBuffer(GL_TEXTURE_BUFFER, 0);	// unbind

			// glBindBuffer(GL_TEXTURE_BUFFER, rt->tbo_materials_id);
			// glActiveTexture(GL_TEXTURE0 + 3);
			// // glBindTexture(GL_TEXTURE_BUFFER, rt->materials_texture_id);
			// glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, rt->tbo_lights_id);
			// uniform_location = glGetUniformLocation(rt->shader_program, "materials");
			// if (uniform_location == -1)
			// 	terminate("lights not found in shader program", 1, rt);
			// glUniform1i(uniform_location, 3);
			// glBindBuffer(GL_TEXTURE_BUFFER, 0);	// unbind

			// RE-BIND AGX
			glBindBuffer(GL_TEXTURE_BUFFER, rt->tbo_agx_lut_id);
			glActiveTexture(GL_TEXTURE0 + 3);
			glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, rt->tbo_agx_lut_id);
			uniform_location = glGetUniformLocation(rt->shader_program, "agx_lut");
			if (uniform_location == -1)
				terminate("agx_lut not found in shader program", 1, rt);
			glUniform1i(uniform_location, 3);
			glBindBuffer(GL_TEXTURE_BUFFER, 0);	// unbind

		}
		
	}
	else if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		reset_camera(&rt->camera);
		rt->move.vel = g_vec3_zero;
	}
	else if (key == GLFW_KEY_UP && action)
	{
		rt->debug += 1;
		printf("debug is %f\n", rt->debug);
	}
	else if (key == GLFW_KEY_DOWN && action)
	{
		rt->debug -= 1;
		printf("debug is %f\n", rt->debug);
	}
	else
		key_hook_axial_view(key, action, rt);
}
