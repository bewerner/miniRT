/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_vertex_buffer_objects.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 15:51:01 by bwerner           #+#    #+#             */
/*   Updated: 2024/10/31 07:19:40 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

void	create_vbo_screen_vertices(t_rt *rt)
{
	GLuint				vbo;
	static const float	vertices[] = {
		1, 1, 0, 1, 0, 0,
		1, -1, 0, 1, 1, 0,
		-1, -1, 0, 0, 1, 0,
		\
		-1, -1, 0, 0, 1, 0,
		-1, 1, 0, 0, 0, 0,
		1, 1, 0, 1, 0, 0
	};

	glGenVertexArrays(1, &rt->vao_screen_id);
	glBindVertexArray(rt->vao_screen_id);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
		(void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}

void	create_vbo_gizmo(t_rt *rt)
{
	GLuint	vbo;

	glGenVertexArrays(1, &rt->vao_gizmo_id);
	glBindVertexArray(rt->vao_gizmo_id);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_gizmo_vertices),
		g_gizmo_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
		(void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}
