/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_environment_map.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 01:05:17 by bwerner           #+#    #+#             */
/*   Updated: 2024/11/28 20:08:54 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

static void apply_strength(double strength, t_vec3 *image, size_t length, t_rt *rt)
{
	(void)rt;
    // // Compute a normalization factor based on max or average luminance
    // double total_luminance = 0.0;
    // for (size_t i = 0; i < length; i++) {
    //     double lum = image[i].r * 0.2126 + image[i].g * 0.7152 + image[i].b * 0.0722;
    //     total_luminance += lum;
    // }
    
    // rt->average_luminance = total_luminance;
    
    for (size_t i = 0; i < length; i++) {
        image[i].r *= strength;
        image[i].g *= strength;
        image[i].b *= strength;
    }
}

void	create_environment_map(t_rt *rt)
{
	int		width;
	int		height;
	size_t	length;
	t_vec3	*image;
	t_vec3	*weights;

	if (rt->ambient.r >= 0)
		return ;
	stbi_set_flip_vertically_on_load(true);
	image = (t_vec3 *)stbi_loadf(rt->ambient_env_file, &width, &height, NULL, 3);
	if (!image)
		terminate(rt->ambient_env_file, NULL, 1, rt);
	weights = (t_vec3 *)malloc(width * height * sizeof(t_vec3));
	if (!weights)
	{
		stbi_image_free(image);
		terminate("environment map weights", NULL, 1, rt);
	}
	length = (size_t)width * height;
	apply_strength(rt->ambient_strength, image, length, rt);
	set_importance_weight(image, weights, width, height, length);
	set_cumulative_distribution(weights, length);
	compensate_weights(weights, length);
	glGenTextures(1, &rt->environment_map_id);
	glActiveTexture(GL_TEXTURE0 + 4);
	glBindTexture(GL_TEXTURE_2D_ARRAY, rt->environment_map_id);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGB32F, width, height, 2, 0, GL_RGB, GL_FLOAT, NULL);
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, 1, GL_RGB, GL_FLOAT, image);
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 1, width, height, 1, GL_RGB, GL_FLOAT, weights);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	stbi_image_free(image);
	free(weights);
}
