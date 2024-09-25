/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_material2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 11:54:03 by nmihaile          #+#    #+#             */
/*   Updated: 2024/09/25 11:54:21 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

void	create_default_material(size_t mat_cnt, t_material *mat)
{
	mat->index = 0;
	if (mat_cnt == 0)
		mat->next = NULL;
	else
		mat->next = (void *)mat + sizeof(t_material);
	ft_strlcat(mat->name, "DEFAULT", MAX_NAME);
	mat->color.r = 204.0f / 255.0f;
	mat->color.g = 204.0f / 255.0f;
	mat->color.b = 204.0f / 255.0f;
	mat->metallic = 0.0f;
	mat->roughness = 0.5f;
	mat->ior = 1.45f;
	mat->transmission = 0.0f;
	mat->emission_strength = 0.0f;
	mat->emission_color.r = 0.0f;
	mat->emission_color.g = 0.0f;
	mat->emission_color.b = 0.0f;
	mat->color_texture_id = -1;
}
