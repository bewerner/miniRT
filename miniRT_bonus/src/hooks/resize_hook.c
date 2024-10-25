/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resize_hook.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 19:46:24 by nmihaile          #+#    #+#             */
/*   Updated: 2024/10/24 07:44:30 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static void	resize_framebuffer(t_rt *rt)
{
	glDeleteFramebuffers(1, &rt->fbo_id);
	glDeleteTextures(1, &rt->tex_fbo_id);
	create_fbo(rt);
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
	resize_framebuffer(rt);
	rt->sample_count = 0;
}
