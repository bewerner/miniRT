/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resize_hook.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 19:46:24 by nmihaile          #+#    #+#             */
/*   Updated: 2024/09/17 21:38:50 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

void resizeFramebufferTexture(t_rt *rt)
{
	// Delete the old textures
	glDeleteTextures(1, &rt->frameTexture);

	// Create new texture with the updated width and height
	glGenTextures(1, &rt->frameTexture);
	glBindTexture(GL_TEXTURE_2D, rt->frameTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, rt->width, rt->height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Reattach the textures to the framebuffer if necessary
	glBindFramebuffer(GL_FRAMEBUFFER, rt->framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rt->frameTexture, 0);

	// Check if the framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("Framebuffer not complete after resize!\n");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void	resize_hook(GLFWwindow *window, int width, int height)
{
	t_rt	*rt;
	float	rad;

	printf("%d\n", width);
	rt = get_rt();
	rad = rt->camera.fov * (M_PI / 180);
	rt->camera.focal_length = 1.0f / tan(rad * 0.5);
	glViewport(0, 0, width, height);
	// glfwGetWindowSize(window, &rt->width, &rt->height);
	// glfwGetWindowContentScale(window, &xscale, &yscale);
	glfwGetFramebufferSize(window, &rt->width, &rt->height);
	resizeFramebufferTexture(rt);
	rt->frame = 0;
}

// void	resize_hook(int width, int height, void *param)
// {
// 	t_rt	*rt;
// 	float	rad;

// 	rt = (t_rt *)param;
// 	rad = rt->camera.fov * (M_PI / 180);
// 	rt->camera.focal_lenth = ((float)width * 0.5) / tanf(rad * 0.5);
// 	mlx_resize_image(rt->canvas, width, height);
// }
