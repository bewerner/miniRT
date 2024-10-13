/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 11:09:09 by nmihaile          #+#    #+#             */
/*   Updated: 2024/10/13 19:30:51 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static t_error	evaluate_id(t_identifier id, t_pobjs objs, t_rt *rt)
{
	if (id == ID_AMBIENT)
		return (parse_ambient(rt));
	else if (id == ID_CAMERA)
		return (parse_camera(rt));
	else if (id == ID_POINT_LIGHT)
		return (parse_point_light((t_point_light *)objs.curr_light, rt));
	else if (id == ID_SPHERE)
		return (parse_sphere((t_sphere *)objs.curr_obj, rt));
	else if (id == ID_PLANE)
		return (parse_plane((t_plane *)objs.curr_obj, rt));
	else if (id == ID_CYLINDER)
		return (parse_cylinder((t_cylinder *)objs.curr_obj, rt));
	else if (id == ID_HYPERBOLOID)
		return (parse_hyperboloid((t_hyperboloid *)objs.curr_obj, rt));
	else if (id != ID_COMMENT && id != ID_MATERIAL && id != ID_TEXTURE)
		return (RT_ERROR_INVALID_IDENTIFIER);
	return (RT_SUCCESS);
}

static void	evaluate_object_id(t_identifier id, size_t obj_cnt,
	t_object **curr_obj)
{
	static size_t	count;

	if (id >= ID_SPHERE)
	{
		count++;
		if (count >= obj_cnt)
			(*curr_obj)->next = NULL;
		else
			*curr_obj = (*curr_obj)->next;
	}
}

static void	evaluate_light_id(t_identifier id, size_t light_cnt,
	t_light **curr_light)
{
	static size_t	count;

	if (id == ID_POINT_LIGHT)
	{
		count++;
		if (count >= light_cnt)
			(*curr_light)->next = NULL;
		else
			*curr_light = (*curr_light)->next;
	}
}

// void	print_textures(t_rt *rt)
// {
// 	t_texture *tex;

// 	tex = rt->textures;
// 	while (tex)
// 	{
// 		printf("-------------------------\n");
// 		printf("INDEX %zu \nNAME %s \nTYPE %i \nFILE %s \n"
// 			"SCALE %f \nCOL1 %f,%f,%f \nCOL2 %f,%f,%f\n",
// 			tex->index,
// 			tex->name,
// 			tex->type,
// 			tex->file,
// 			tex->scale,
// 			tex->col1.r,
// 			tex->col1.g,
// 			tex->col1.b,
// 			tex->col2.r,
// 			tex->col2.g,
// 			tex->col2.b);
// 		tex = tex->next;
// 	}
// 	printf("\n\n");
// }

// void	print_materialss(t_rt *rt)
// {
// 	t_material *mat;

// 	printf("###############\nMATERIALS\n###############\n");
// 	mat = rt->materials;
// 	while (mat)
// 	{
// 		printf("-------------------------\n");
// 		printf("INDEX %zu \nNAME %s \nCOLOR %f,%f,%f \nMETALLIC %f \n"
// 			"ROUGHNESS %f \nIOR %f\nTRANSMISSION %f\nEMMISSION STRENGTH %f\n"
// 			"EMMISSION COL %f,%f,%f \nCOLOR_TEXTURE_IDX %i\n",
// 			mat->index,
// 			mat->name,
// 			mat->color.r,
// 			mat->color.g,
// 			mat->color.b,
// 			mat->metallic,
// 			mat->roughness,
// 			mat->ior,
// 			mat->transmission,
// 			mat->emission_strength,
// 			mat->emission_color.r,
// 			mat->emission_color.g,
// 			mat->emission_color.b,
// 			mat->color_texture_id);
// 		mat = mat->next;
// 	}
// 	printf("\n\n");
// }
// 	print_materialss(rt);
// 	print_textures(rt);

t_error	parse_scene(t_scene scene, t_rt *rt)
{
	t_identifier	id;
	t_error			error;
	t_object		*curr_obj;
	t_light			*curr_light;

	error = RT_SUCCESS;
	curr_obj = rt->objects;
	curr_light = rt->lights;
	create_textures(scene.tex_cnt, rt);
	create_materials(scene.mat_cnt, rt);
	while (rt->line)
	{
		rt->curr_line = rt->line->content;
		id = get_identifier(rt->line->content);
		error = evaluate_id(id, (t_pobjs){curr_obj, curr_light}, rt);
		if (error)
			return (error);
		evaluate_object_id(id, scene.obj_cnt, &curr_obj);
		evaluate_light_id(id, scene.light_cnt, &curr_light);
		next_lst_item(&rt->line);
	}
	verify_material_uniqueness(rt);
	verify_texture_uniqueness(rt);
	return (RT_SUCCESS);
}
