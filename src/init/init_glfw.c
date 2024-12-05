/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_glfw.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 16:02:03 by bwerner           #+#    #+#             */
/*   Updated: 2024/12/05 16:32:21 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

void	init_glfw(t_rt *rt)
{
	glfwInitHint(GLFW_COCOA_CHDIR_RESOURCES, GLFW_FALSE);
	if (glfwInit() == GLFW_FALSE)
		terminate("glfw_init failed", NULL, 1, rt);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	rt->window = glfwCreateWindow(rt->width, rt->height, "miniRT - LOADING...", NULL, NULL);
	if (!rt->window)
		terminate("glfw window creation failed", NULL, 1, rt);
	glfwMakeContextCurrent(rt->window);
	glfwShowWindow(rt->window);
	glfwPollEvents();
	glfwGetFramebufferSize(rt->window, &rt->width, &rt->height);
	rt->aspect_ratio = (float)rt->width / (float)rt->height;
	glfwGetWindowContentScale(rt->window, &rt->dpi_scale, NULL);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);
}
