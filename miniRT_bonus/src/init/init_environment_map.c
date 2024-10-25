/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_environment_map.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 01:05:17 by bwerner           #+#    #+#             */
/*   Updated: 2024/10/21 16:22:18 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

void	apply_strength(double strength, t_vec4 *image, int width, int height)
{
	size_t	length;
	size_t	i;

	length = (size_t)width * height;
	i = 0;
	while (i < length)
	{
		image[i].r *= strength;
		image[i].g *= strength;
		image[i].b *= strength;
		i++;
	}
}

void	create_environment_map(t_rt *rt)
{
	int		width;
	int		height;
	t_vec4	*image;

	if (rt->ambient.r >= 0)
		return ;
	stbi_set_flip_vertically_on_load(true);
	image = (t_vec4 *)stbi_loadf(
			rt->ambient_env_file, &width, &height, NULL, 4);
	if (!image)
		terminate(rt->ambient_env_file, NULL, 1, rt);
	apply_strength(rt->ambient_strength, image, width, height);
	set_importance_weight(image, width, height);
	set_cumulative_distribution(image, width, height);
	glGenTextures(1, &rt->environment_map_id);
	glActiveTexture(GL_TEXTURE0 + 4);
	glBindTexture(GL_TEXTURE_2D, rt->environment_map_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0,
		GL_RGBA, GL_FLOAT, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(image);
}
