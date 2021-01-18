/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@kiborroq.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 23:10:31 by kiborroq          #+#    #+#             */
/*   Updated: 2021/01/15 11:50:54 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/cub3d.h"

int		init_sprs(t_inf *inf)
{
	int	x;
	int	y;
	int	i;

	inf->num_sprs = ft_numdig(inf->map, inf->map_height, inf->map_width, SPR);
	if (!(inf->sprs = (t_spr *)malloc(inf->num_sprs * sizeof(t_spr))))
		return (KO);
	x = -1;
	i = 0;
	while (++x < inf->map_height && i < inf->num_sprs)
	{
		y = -1;
		while (++y < inf->map_width && i < inf->num_sprs)
		{
			if (inf->map[x][y] == SPR)
			{
				inf->sprs[i].abs_pos.x = x + 0.5;
				inf->sprs[i].abs_pos.y = y + 0.5;
				inf->sprs[i].dist = 0.0;
				i++;
			}
		}
	}
	return (OK);
}

void	init_view(t_inf *inf)
{
	int		i;
	int		j;
	char	*dir;

	i = 0;
	while (i < inf->map_height)
	{
		j = 0;
		while (j < inf->map_width)
		{
			if ((dir = ft_strchr(START_POS, inf->map[i][j] + '0')) != 0)
			{
				set_pos_dir_cam(inf, i + 0.5, j + 0.5, *dir);
				inf->map[i][j] = 0;
				return ;
			}
			j++;
		}
		i++;
	}
}

void	init_speed(t_inf *inf)
{
	inf->speed.move = inf->width * inf->height / (double)MOVE_SPEED_DEL;
	inf->speed.turn = inf->width * inf->height / (double)TURN_SPEED_DEL;
	inf->turn.cos_negative = cos(-inf->speed.turn);
	inf->turn.cos_pozitive = cos(inf->speed.turn);
	inf->turn.sin_negative = sin(-inf->speed.turn);
	inf->turn.sin_pozotive = sin(inf->speed.turn);
}

int		init_inf_mlx(t_inf **inf)
{
	t_inf *tmp;

	if (!(tmp = (t_inf *)ft_calloc(1, sizeof(t_inf))))
	{
		*inf = 0;
		return (KO);
	}
	*inf = tmp;
	if (!(tmp->mlx = mlx_init()))
		return (KO);
	mlx_get_screen_size(tmp->mlx, &tmp->max_width, &tmp->max_height);
	if (!(tmp->dist_buf = (double *)ft_calloc(tmp->max_width, sizeof(double))))
		return (KO);
	return (OK);
}

int		init_img(t_inf *inf)
{
	if (!(inf->img.img_ptr = mlx_new_image(inf->mlx, inf->width, inf->height)))
		return (KO);
	inf->img.height = inf->height;
	inf->img.width = inf->width;
	inf->img.data = (int *)mlx_get_data_addr(inf->img.img_ptr,
											&inf->img.bpp,
											&inf->img.sl,
											&inf->img.endian);
	return (OK);
}
