/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_environment_map.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 01:05:17 by bwerner           #+#    #+#             */
/*   Updated: 2024/10/01 00:55:26 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

void	set_cumulative_distribution(t_vec4 *image, int width, int height)
{
	size_t	length;
	size_t	i;
	float	previous_weight;

	length = (size_t)width * height;
	i = 0;
	previous_weight = 0;
	while (i < length)
	{
		image[i].a += previous_weight;
		previous_weight = image[i].a;
		i++;
	}
}

void	normalize_weight(t_vec4 *image, float total_weight, int width, int height)
{
	size_t	length;
	size_t	i;

	length = (size_t)width * height;
	i = 0;
	while (i < length)
	{
		image[i].a /= total_weight;
		i++;
	}
}

void	apply_strength(float strength, t_vec4 *image, int width, int height)
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

float	get_importance_weight(float luminance, float row, int width, int height)
{
	float	solid_angle;
	float	importance_weight;
	float	pi = M_PI;

	solid_angle = sinf(row / height * pi) * (pi / height) * (2 * pi / width);
	importance_weight = luminance * solid_angle;
	return (importance_weight);
}

void	set_importance_weight(t_vec4 *image, int width, int height)
{
	t_vec4	*pixel;
	int		x;
	int		y;
	float	total_weight;

	total_weight = 0;
	y = 0;
	while (y < height)
	{
		x = 0;
		while (x < width)
		{
			pixel = &image[x + y * width];
			pixel->a = pixel->r * 0.2126 + pixel->g * 0.7152 + pixel->b * 0.0722;
			pixel->a = get_importance_weight(pixel->a, y + 1, width, height);
			total_weight += pixel->a;
			x++;
		}
		y++;
	}
	normalize_weight(image, total_weight, width, height);
}

void	create_environment_map(t_rt *rt)
{
	int		width;
	int		height;
	t_vec4	*image;
	// size_t	length;

	if (rt->ambient.r >= 0)
		return ;
	stbi_set_flip_vertically_on_load(true);
	image = (t_vec4 *)stbi_loadf(rt->ambient_env_file, &width, &height, NULL, 4);
	if (!image)
		terminate(rt->ambient_env_file, NULL, 1, rt);

	// length = (size_t)width * height;
	apply_strength(rt->ambient_strength, image, width, height);
	set_importance_weight(image, width, height);
	set_cumulative_distribution(image, width, height);
	// set_importance_weight(image, width, height);

	// calc_luminance(image, length);
	// Luminance=0.2126×Red+0.7152×Green+0.0722×Blue

	// set_importance_weight(image, length);
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0,
		GL_RGBA, GL_FLOAT, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(image);
}
