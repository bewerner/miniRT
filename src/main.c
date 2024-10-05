/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 15:10:10 by nmihaile          #+#    #+#             */
/*   Updated: 2024/10/05 03:17:20 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define STB_IMAGE_IMPLEMENTATION
#include "../inc/miniRT.h"

// // Convert a direction vector to UV coordinates
// t_vec2 direction_to_uv(t_vec3 direction) {
// 	t_vec2 uv;
// 	t_vec3 normal = vec3_normalize(direction);
// 	uv.x = 0.5 + atan2(-normal.y, normal.x) / (2.0 * M_PI);
// 	uv.y = 0.5 - asin(-normal.z) / M_PI;
// 	return uv;
// }

// // Convert UV coordinates back to a direction vector
// t_vec3 uv_to_direction(t_vec2 uv) {
// 	t_vec3 direction;

// 	// Convert the UV coordinates back to angles
// 	float theta = (uv.x - 0.5) * 2.0 * M_PI; // Angle in the XY plane
// 	float phi = (0.5 - uv.y) * M_PI;         // Angle from the Z axis

// 	// Calculate the direction vector using spherical coordinates
// 	direction.x = cos(phi) * cos(theta);   // x = cos(phi) * cos(theta)
// 	direction.y = -cos(phi) * sin(theta);   // y = cos(phi) * sin(theta)
// 	direction.z = -sin(phi);                  // z = sin(phi)

// 	return (direction); // Ensure it is a normalized direction
// 	// return vec3_normalize(direction); // Ensure it is a normalized direction
// }

// t_vec3  lerp(t_vec3 a, t_vec3 b, float f)
// {
// 	return (vec3_add(vec3_scale(1.0f - f, a), vec3_scale(f, b)));
// }

	// t_vec3 direction = (t_vec3){{5, 2, 1}};
	// direction = vec3_normalize(direction);
	// printf("Normalized direction: %f, %f, %f\n", direction.x, direction.y, direction.z);
	
	// t_vec2 uv = direction_to_uv(direction);
	// printf("UV: %f, %f\n", uv.x, uv.y);
	
	// direction = uv_to_direction(uv);
	// printf("Direction from UV: %f, %f, %f\n", direction.x, direction.y, direction.z);

	// t_vec3 normal = (t_vec3){{0, 0, 1}};
	// normal = vec3_normalize(normal);
	// printf("Normal:    %f, %f, %f\n\n", normal.x, normal.y, normal.z);

	// t_vec3 texNormal = (t_vec3){{5, 2, 1}};
	// texNormal = vec3_normalize(texNormal);
	// printf("texNormal: %f, %f, %f\n\n", texNormal.x, texNormal.y, texNormal.z);

	// t_vec3 outNormal;

	// printf("outNormal = lerp(normal, texNormal, 99.0f);\n");
	// outNormal = lerp(normal, texNormal, 99.0f);

	// printf("outNormal: %f, %f, %f\n\n", outNormal.x, outNormal.y, outNormal.z);
	// outNormal = vec3_normalize(outNormal);

	// printf("normalize(outNormal);\n");
	// printf("outNormal: %f, %f, %f\n\n", outNormal.x, outNormal.y, outNormal.z);
	
	// return (0);

void	validate_args(int argc, char **argv, t_rt *rt)
{
	if (argc != 2)
		terminate("usage: ./miniRt scene_file.rt", NULL, 1, rt);
	if (ft_strlen(argv[1]) <= 3
		|| !ft_strrchr(argv[1], '.')
		|| ft_strncasecmp(ft_strrchr(argv[1], '.'), ".rt\0", 4))
	{
		terminate(error_msg(RT_ERROR_WRONG_FILE_FORMAT), NULL, 1, rt);
	}
}

t_rt	*get_rt(void)
{
	static t_rt	rt;

	return (&rt);
}

int	main(int argc, char **argv)
{
	t_rt	*rt;
	rt = get_rt();
	errno = 0;
	validate_args(argc, argv, rt);
	init_mini_rt(argv, rt);
	rt->mode = MODE_PREVIEW;
	while (!glfwWindowShouldClose(rt->window))
		update(rt);
	terminate(NULL, NULL, 0, rt);
	return (0);
}
