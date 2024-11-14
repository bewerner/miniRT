/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resize_hook.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 19:46:24 by nmihaile          #+#    #+#             */
/*   Updated: 2024/11/14 06:19:19 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

void	resize_framebuffer(t_rt *rt, float render_scale)
{
	glDeleteFramebuffers(1, &rt->fbo_id);
	glDeleteTextures(1, &rt->tex_fbo_id);
	create_fbo(rt, render_scale);
	rt->sample_count = 0;
	glFinish();
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
	resize_framebuffer(rt, rt->render_scale);
	rt->sample_count = 0;
}
