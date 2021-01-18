/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@kiborroq.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 23:10:31 by kiborroq          #+#    #+#             */
/*   Updated: 2021/01/15 11:51:32 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/cub3d.h"

void	draw_stripe(t_inf *inf, int screen_x)
{
	inf->color.tmp = inf->color.ceil;
	draw_color_range(inf, screen_x, 0, inf->ray.wall_start - 1);
	draw_wall(inf, screen_x);
	inf->color.tmp = inf->color.floor;
	draw_color_range(inf, screen_x, inf->ray.wall_end, inf->height - 1);
}

void	calculate_wall_range(t_inf *inf)
{
	int height;
	int start;
	int end;

	height = (int)(inf->height / inf->ray.dist);
	if ((start = -height / 2 + inf->height / 2) < 0)
		start = 0;
	if ((end = height / 2 + inf->height / 2) >= inf->height)
		end = inf->height - 1;
	inf->ray.wall_height = height;
	inf->ray.wall_start = start;
	inf->ray.wall_end = end;
}

void	calculate_dist_to_wall(t_inf *inf, int screen_x)
{
	while (1)
	{
		if (inf->ray.cur_dist.x < inf->ray.cur_dist.y)
		{
			inf->ray.cur_dist.x += inf->ray.delt_dist.x;
			inf->ray.pos.x += inf->ray.step.x;
			inf->ray.hit_side = 0;
		}
		else
		{
			inf->ray.cur_dist.y += inf->ray.delt_dist.y;
			inf->ray.pos.y += inf->ray.step.y;
			inf->ray.hit_side = 1;
		}
		if (inf->map[inf->ray.pos.x][inf->ray.pos.y] == WALL)
			break ;
	}
	if (inf->ray.hit_side == 0)
		inf->ray.dist = (inf->ray.pos.x - inf->view.pos.x +
						(1 - inf->ray.step.x) / 2) / inf->ray.dir.x;
	else
		inf->ray.dist = (inf->ray.pos.y - inf->view.pos.y +
						(1 - inf->ray.step.y) / 2) / inf->ray.dir.y;
	inf->dist_buf[screen_x] = inf->ray.dist;
}

void	calculate_step(t_inf *inf)
{
	if (inf->ray.dir.x < 0)
	{
		inf->ray.step.x = -1;
		inf->ray.cur_dist.x = (inf->view.pos.x - inf->ray.pos.x) *
							inf->ray.delt_dist.x;
	}
	else
	{
		inf->ray.step.x = 1;
		inf->ray.cur_dist.x = (inf->ray.pos.x + 1.0 - inf->view.pos.x) *
							inf->ray.delt_dist.x;
	}
	if (inf->ray.dir.y < 0)
	{
		inf->ray.step.y = -1;
		inf->ray.cur_dist.y = (inf->view.pos.y - inf->ray.pos.y) *
							inf->ray.delt_dist.y;
	}
	else
	{
		inf->ray.step.y = 1;
		inf->ray.cur_dist.y = (inf->ray.pos.y + 1.0 - inf->view.pos.y) *
							inf->ray.delt_dist.y;
	}
}

void	init_ray(t_inf *inf, int screen_x)
{
	double pos_on_plane;

	pos_on_plane = 2 * screen_x / (double)inf->width - 1;
	inf->ray.dir.x = inf->view.dir.x + pos_on_plane * inf->view.cam.x;
	inf->ray.dir.y = inf->view.dir.y + pos_on_plane * inf->view.cam.y;
	inf->ray.pos.x = (int)inf->view.pos.x;
	inf->ray.pos.y = (int)inf->view.pos.y;
	if (inf->ray.dir.y != 0)
		inf->ray.delt_dist.x = inf->ray.dir.x == 0 ?
								1 : fabs(1 / inf->ray.dir.x);
	else
		inf->ray.delt_dist.x = 0;
	if (inf->ray.dir.x != 0)
		inf->ray.delt_dist.y = inf->ray.dir.y == 0 ?
								1 : fabs(1 / inf->ray.dir.y);
	else
		inf->ray.delt_dist.y = 0;
}
