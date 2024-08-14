/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_errors.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 13:11:26 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/14 23:17:34 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_ERRORS_H
# define MINIRT_ERRORS_H

# define RT_ERROR_MSG_SUCCES \
"all good"
# define RT_ERROR_MSG_WRONG_FILE_FORMAT \
"wrong file format. (needs to be .rt)"
# define RT_ERROR_MSG_INVALID_IDENTIFIER \
"invalid identifier"
# define RT_ERROR_MSG_CAMERA_LOWER_CASE \
"camera defined in lower case, only 1 camera allowd in \
.rt scenes. Please use capital C"
# define RT_ERROR_MSG_MALLOC \
"faile to allocate memory"
# define RT_ERROR_MSG_TOO_MANY_LIGHTS \
"too many lights, use lowercase for multiple lights, \
force one light with capital L"
# define RT_ERROR_MSG_TOO_MANY_CAMERAS \
"too many cameras, only one allowed"
# define RT_ERROR_MSG_TOO_MANY_AMBIENT \
"too many ambient definitions, only one allowed"

typedef enum e_error
{
	RT_SUCCESS,
	RT_ERROR_WRONG_FILE_FORMAT,
	RT_ERROR_INVALID_IDENTIFIER,
	RT_ERROR_MALLOC,
	RT_ERROR_TOO_MANY_LIGHTS,
	RT_ERROR_TOO_MANY_CAMERAS,
	RT_ERROR_TOO_MANY_AMBIENT
}					t_error;

#endif
