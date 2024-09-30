/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_environment_map.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 01:05:17 by bwerner           #+#    #+#             */
/*   Updated: 2024/09/30 02:28:25 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

void	apply_strength(float strength, t_vec3 *image, size_t width, size_t height)
{
	size_t	x;
	size_t	y;

	y = 0;
	while (y < height)
	{
		x = 0;
		while (x < width)
		{
			image[x + y * width] = vec3_scale(strength, image[x + y * width]);
			x++;
		}
		y++;
	}
}

void	create_environment_map(t_rt *rt)
{
	int		width;
	int		height;
	t_vec3	*image;
	// size_t	length;

	if (rt->ambient.r >= 0)
		return ;
	stbi_set_flip_vertically_on_load(true);
	image = (t_vec3 *)stbi_loadf(rt->ambient_env_file, &width, &height, NULL, 3);
	if (!image)
		terminate(rt->ambient_env_file, NULL, 1, rt);


	// length = (size_t)width * height;
	apply_strength(rt->ambient_strength, image, width, height);

	// calc_luminance(image, length);
	// Luminance=0.2126×Red+0.7152×Green+0.0722×Blue

	// calc_importance_weight(image, length);
	// consider actual area of each pixel on the skysphere to create area weighted importance

	// PDF (Probability Distribution Function)
	// normalize importance weights so the sum of all weights add up to 1.0

	// CDF (Cumulative Distribution Function):
	// first pixel has its weight, second pixel has first pixels weight + second pixels weight,....,last pixel has 1.0

	// in the shader, do rand() and sample the env_map at the pixel with the corresponding weight.
	// while (pixel.weight <= random_value)
	//     next pixel;
	// (optimize this search algorithm later)

	// get ray direction which points to this pixel

	// contribution = color / importance_weight;
	// new_accumulated_color = mix(old_accumulated_color, new_sample_value_color / PDF_of_new_sample, 1.0 / sample_count);
	// ??not sure about this
	// chatgpt says not to use mix, instead:
	// accumulated_color += new_sample_value_color / PDF_of_new_sample;
	// should we accumulate in our framebuffer, and only divide by sample count in postprocessing?

	glGenTextures(1, &rt->environment_map_id);
	glActiveTexture(GL_TEXTURE0 + 4);
	glBindTexture(GL_TEXTURE_2D, rt->environment_map_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0,
		GL_RGB, GL_FLOAT, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(image);
}
