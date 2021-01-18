/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sprites.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@kiborroq.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 23:10:31 by kiborroq          #+#    #+#             */
/*   Updated: 2021/01/15 11:50:05 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/cub3d.h"

void	draw_curr_spr(t_spr *spr, t_image *spr_img, t_inf *inf)
{
	int		x;
	int		y;
	double	textr_step;
	int		textr_x;
	int		textr_y;

	x = spr->start.x - 1;
	textr_step = 1.0 * spr_img->height / spr->height;
	while (++x < spr->end.x)
	{
		textr_x = (int)(spr_img->sl * (x - (-spr->width / 2 + spr->screen_x)) *
						spr_img->width / spr->width) / spr_img->sl;
		y = spr->start.y - 1;
		if (spr->transform.y > 0 && x > 0 && x < inf->width &&
			spr->transform.y < inf->dist_buf[x])
		{
			while (++y < spr->end.y)
			{
				textr_y = (y - inf->height / 2 + spr->height / 2) * textr_step;
				if ((inf->color.tmp = get_color(spr_img, textr_x, textr_y))
					!= TRANSPARENT)
					put_color(&inf->img, x, y, inf->color.tmp);
			}
		}
	}
}

void	calculate_spr_range(t_spr *spr, int width, int height)
{
	spr->height = abs((int)(height / spr->transform.y));
	if ((spr->start.y = -spr->height / 2 + height / 2) < 0)
		spr->start.y = 0;
	if ((spr->end.y = spr->height / 2 + height / 2) >= height)
		spr->end.y = height - 1;
	spr->width = spr->height;
	if ((spr->start.x = -spr->width / 2 + spr->screen_x) < 0)
		spr->start.x = 0;
	if ((spr->end.x = spr->width / 2 + spr->screen_x) >= width)
		spr->end.x = width - 1;
}

void	transform_spr(t_spr *spr, t_inf *inf)
{
	t_point_d	rel_pos;
	double		inv_det;

	rel_pos.x = spr->abs_pos.x - inf->view.pos.x;
	rel_pos.y = spr->abs_pos.y - inf->view.pos.y;
	inv_det = 1.0 / (inf->view.cam.x * inf->view.dir.y -
					inf->view.cam.y * inf->view.dir.x);
	spr->transform.x = inv_det * (inf->view.dir.y * rel_pos.x -
								inf->view.dir.x * rel_pos.y);
	spr->transform.y = inv_det * (-inf->view.cam.y * rel_pos.x +
								inf->view.cam.x * rel_pos.y);
	spr->screen_x = (int)((inf->width / 2) *
					(1 + spr->transform.x / spr->transform.y));
}

void	count_dist_to_spr(t_inf *inf)
{
	int	i;
	int	delta_x;
	int delta_y;

	i = 0;
	while (i < inf->num_sprs)
	{
		delta_x = abs((int)(inf->sprs[i].abs_pos.x - inf->view.pos.x));
		delta_y = abs((int)(inf->sprs[i].abs_pos.y - inf->view.pos.y));
		inf->sprs[i].dist = sqrt(delta_y * delta_y + delta_x * delta_x);
		i++;
	}
}

void	draw_sprs(t_inf *inf)
{
	int i;

	i = 0;
	count_dist_to_spr(inf);
	sort_sprs(inf->sprs, inf->num_sprs);
	while (i < inf->num_sprs)
	{
		transform_spr(inf->sprs + i, inf);
		calculate_spr_range(inf->sprs + i, inf->width, inf->height);
		draw_curr_spr(inf->sprs + i, &inf->textr.spr, inf);
		i++;
	}
}
