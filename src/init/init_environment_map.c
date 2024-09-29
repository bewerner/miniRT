/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_environment_map.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 01:05:17 by bwerner           #+#    #+#             */
/*   Updated: 2024/09/30 01:05:34 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

void	create_environment_map(t_rt *rt)
{
	int		width;
	int		height;
	t_vec3	*image;
	size_t	i;

	if (rt->ambient.r >= 0)
		return ;
	stbi_set_flip_vertically_on_load(true);
	image = (t_vec3 *)stbi_loadf(rt->ambient_env_file, &width, &height, NULL, 3);
	if (!image)
		terminate(rt->ambient_env_file, NULL, 1, rt);
	i = 0;
	while (i < (size_t)width * height)
	{
		image[i] = vec3_scale(rt->ambient_strength, image[i]);
		i++;
	}
	glGenTextures(1, &rt->environment_map_id);
	glActiveTexture(GL_TEXTURE0 + 4);
	glBindTexture(GL_TEXTURE_2D, rt->environment_map_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0,
		GL_RGB, GL_FLOAT, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(image);
}
