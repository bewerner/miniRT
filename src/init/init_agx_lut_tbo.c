/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_agx_lut_tbo.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 16:21:54 by nmihaile          #+#    #+#             */
/*   Updated: 2024/09/22 16:25:37 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static float	get_next_value(int fd)
{
	char	str[9];
	float	value;

	read(fd, str, 9);
	str[1] = '\0';
	str[8] = '\0';
	value = ft_atoi(str);
	value += (float)ft_atoi(str + 2) / 1000000;
	return (value);
}

static void	init_agx_lut_buffer(t_vec3 *buffer, char *filepath,
	size_t len, t_rt *rt)
{
	int		fd;
	size_t	i;

	fd = open(filepath, O_RDONLY);
	if (fd == -1)
		terminate(filepath, NULL, 1, rt);
	i = 0;
	while (i < len)
	{
		buffer[i].x = get_next_value(fd);
		buffer[i].y = get_next_value(fd);
		buffer[i].z = get_next_value(fd);
		i++;
	}
	close(fd);
}

void	create_tbo_agx_lut(char *filepath, t_rt *rt)
{
	static const size_t	resolution = 64 * 64 * 64;
	size_t				size;
	t_vec3				buffer[resolution];
	GLuint				texture_id;
	// GLuint	tbo_agx_lut_id;

	size = resolution * sizeof(t_vec3);
	init_agx_lut_buffer(buffer, filepath, resolution, rt);

	// for (size_t i = 0; i < resolution; i++)
	// 	printf("%f\n", buffer[i]);

	glGenBuffers(1, &rt->tbo_agx_lut_id);
	glBindBuffer(GL_TEXTURE_BUFFER, rt->tbo_agx_lut_id);
	glBufferData(GL_TEXTURE_BUFFER, size, buffer, GL_STATIC_DRAW);
	glBindBuffer(GL_TEXTURE_BUFFER, 0);

	glGenTextures(1, &texture_id);
	glActiveTexture(GL_TEXTURE0 + 3);
	glBindTexture(GL_TEXTURE_BUFFER, texture_id);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, rt->tbo_agx_lut_id);
}
