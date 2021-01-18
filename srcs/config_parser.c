/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@kiborroq.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 23:10:31 by kiborroq          #+#    #+#             */
/*   Updated: 2021/01/15 12:53:38 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/cub3d.h"

int	check_elem(t_inf *inf, char *elem)
{
	int status;

	status = KO;
	elem = ft_skip_spaces(elem);
	if (*elem == '\0')
		status = 0;
	else if (ismap(elem, FIRST_LAST_LINE))
		inf->message = NOT_ENOUGH_CONFIG;
	else
		inf->message = UNKNOWN_ELEM;
	return (status);
}

int	init_resltn(t_inf *inf, char *elem)
{
	int status;

	status = 0;
	if (elem[0] == 'R')
	{
		if (inf->width == 0 && inf->height == 0)
			status = add_resltn(inf, &inf->width, &inf->height, elem + 1);
		else
		{
			status = KO;
			inf->message = ELEM_REPAET;
		}
	}
	if (status == KO && inf->message == 0)
		inf->message = RESOLUTION_ERROR;
	return (status);
}

int	init_rgb(t_inf *inf, char *elem)
{
	int status;

	status = 0;
	if (elem[0] == 'F')
		status = add_rgb_wrap(inf, &inf->color.floor, elem);
	else if (elem[0] == 'C')
		status = add_rgb_wrap(inf, &inf->color.ceil, elem);
	if (status == KO && inf->message == 0)
		inf->message = COLOR_ERROR;
	return (status);
}

int	init_textr(t_inf *inf, char *elem)
{
	int		status;

	status = 0;
	if (!ft_strncmp(elem, "EA", 2))
		status = add_textr_wrap(inf, &inf->textr.e_side, elem + 2);
	else if (!ft_strncmp(elem, "WE", 2))
		status = add_textr_wrap(inf, &inf->textr.w_side, elem + 2);
	else if (!ft_strncmp(elem, "SO", 2))
		status = add_textr_wrap(inf, &inf->textr.s_side, elem + 2);
	else if (!ft_strncmp(elem, "NO", 2))
		status = add_textr_wrap(inf, &inf->textr.n_side, elem + 2);
	else if (!ft_strncmp(elem, "S", 1))
		status = add_textr_wrap(inf, &inf->textr.spr, elem + 1);
	if (status == KO && inf->message == 0)
		inf->message = TEXTURE_ERROR;
	return (status);
}
