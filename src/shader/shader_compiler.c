/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader_compiler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 10:31:58 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/25 19:47:16 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

GLuint	compile_shader_src(GLenum shader_type, const char *shader_src)
{
	GLuint	shader_id;
	int		success;
	char	errorlog[512];

	if (shader_src == NULL)
		return (-1);
	shader_id = glCreateShader(shader_type);
	glShaderSource(shader_id, 1, &shader_src, NULL);
	glCompileShader(shader_id);
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE)
	{
		glGetShaderInfoLog(shader_id, 512, NULL, errorlog);
		printf("ERROR SHADER COMPILATION FAILED\n%s\n", errorlog);
		return (0);
	}
	return (shader_id);
}
