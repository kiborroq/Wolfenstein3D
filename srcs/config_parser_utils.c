/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_parser_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@kiborroq.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 23:10:31 by kiborroq          #+#    #+#             */
/*   Updated: 2021/01/15 12:54:22 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/cub3d.h"

int	add_textr(void *mlx, t_image *textr, char *path)
{
	if (!(textr->img_ptr = mlx_xpm_file_to_image(mlx, path, &textr->width,
												&textr->height)))
	{
		return (KO);
	}
	textr->data = (int *)mlx_get_data_addr(textr->img_ptr,
											&textr->bpp,
											&textr->sl,
											&textr->endian);
	return (OK);
}

int	add_textr_wrap(t_inf *inf, t_image *textr, char *elem)
{
	char *path;

	path = ft_skip_spaces(elem);
	if (path > elem)
	{
		if (textr->img_ptr == 0)
			return (add_textr(inf->mlx, textr, path));
		else
			inf->message = ELEM_REPAET;
	}
	return (KO);
}

int	add_rgb(int *color, char *rgb)
{
	int		i;
	int		num;
	char	*curr;

	i = 0;
	while (i < 3 && *rgb)
	{
		if ((num = ft_atoi(rgb)) > 255 || num < 0)
			break ;
		if ((curr = ft_skip_spaces(rgb)) == rgb && i == 0)
			break ;
		rgb = curr;
		if ((curr = ft_skip_digits(rgb)) == rgb)
			break ;
		curr = ft_skip_spaces(curr);
		if (i < 2 && *curr != ',')
			break ;
		if (i < 2 && *curr == ',')
			curr++;
		rgb = curr;
		*color = (*color << 8) + num;
		i++;
	}
	return (i < 3 || *rgb ? KO : OK);
}

int	add_rgb_wrap(t_inf *inf, int *color, char *elem)
{
	int status;

	if (*color == 0)
		status = add_rgb(color, elem + 1);
	else
	{
		status = KO;
		inf->message = ELEM_REPAET;
	}
	return (status);
}

int	add_resltn(t_inf *inf, int *width, int *height, char *resltn)
{
	int		i;
	int		num;
	char	*curr;

	i = 0;
	while (i < 2 && *resltn)
	{
		if ((num = ft_atoi(resltn)) < 0)
			break ;
		if ((curr = ft_skip_spaces(resltn)) == resltn)
			break ;
		resltn = curr;
		if ((curr = ft_skip_digits(resltn)) == resltn)
			break ;
		resltn = curr;
		if (i == 0)
			*width = num > inf->max_width ? inf->max_width : num;
		else
			*height = num > inf->max_height ? inf->max_height : num;
		i++;
	}
	if (i < 2 || *resltn)
		return (KO);
	return (OK);
}
