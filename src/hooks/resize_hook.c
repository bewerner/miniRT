/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resize_hook.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 19:46:24 by nmihaile          #+#    #+#             */
/*   Updated: 2024/09/28 14:07:19 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

void	resize_framebuffer_texture(t_rt *rt)
{
	glDeleteTextures(1, &rt->tex_fbo_id);
	glGenTextures(1, &rt->tex_fbo_id);
	glBindTexture(GL_TEXTURE_2D, rt->tex_fbo_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, rt->width, rt->height, 0,
		GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, rt->fbo_id);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, rt->tex_fbo_id, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("Framebuffer not complete after resize!\n");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void	resize_hook(GLFWwindow *window, int width, int height)
{
	t_rt	*rt;
	float	rad;

	rt = get_rt();
	rad = rt->camera.fov * (M_PI / 180);
	rt->camera.focal_length = 1.0f / tan(rad * 0.5);
	glViewport(0, 0, width, height);
	glfwGetFramebufferSize(window, &rt->width, &rt->height);
	rt->aspect_ratio = (float)rt->width / (float)rt->height;
	resize_framebuffer_texture(rt);
	rt->sample_count = 0;
}
