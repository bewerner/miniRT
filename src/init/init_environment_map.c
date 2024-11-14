/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_environment_map.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 01:05:17 by bwerner           #+#    #+#             */
/*   Updated: 2024/11/14 19:57:17 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

void	apply_strength(double strength, t_vec3 *env_map, int width, int height)
{
	size_t	length;
	size_t	i;

	length = (size_t)width * height;
	i = 0;
	while (i < length)
	{
		env_map[i].r *= strength;
		env_map[i].g *= strength;
		env_map[i].b *= strength;
		i++;
	}
}

void	create_environment_map(t_rt *rt)
{
	int		width;
	int		height;
	t_vec3	*env_map;
	t_vec3	*weights;

	if (rt->ambient.r >= 0)
		return ;
	stbi_set_flip_vertically_on_load(true);
	env_map = (t_vec3 *)stbi_loadf(rt->ambient_env_file, &width, &height, NULL, 3);
	if (!env_map)
		terminate(rt->ambient_env_file, NULL, 1, rt);
	weights = (t_vec3 *)malloc(width * height * sizeof(t_vec3));
	if (!weights)
	{
		stbi_image_free(env_map);
		terminate("environment map weights", NULL, 1, rt);
	}
	apply_strength(rt->ambient_strength, env_map, width, height);
	set_importance_weight(env_map, weights, width, height);
	set_cumulative_distribution(weights, width, height);
	compensate_weights(weights, width, height);
	glGenTextures(1, &rt->environment_map_id);
	glActiveTexture(GL_TEXTURE0 + 4);
	glBindTexture(GL_TEXTURE_2D_ARRAY, rt->environment_map_id);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGB32F, width, height, 2, 0, GL_RGB, GL_FLOAT, NULL);
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, 1, GL_RGB, GL_FLOAT, env_map);
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 1, width, height, 1, GL_RGB, GL_FLOAT, weights);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(env_map);
	free(weights);
}
