/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_wall_ceil_floor.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@kiborroq.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 23:10:31 by kiborroq          #+#    #+#             */
/*   Updated: 2021/01/15 11:50:17 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/cub3d.h"

double	get_wall_x(t_inf *inf)
{
	double wall_x;

	if (inf->ray.hit_side == 1)
		wall_x = inf->view.pos.x + inf->ray.dist * inf->ray.dir.x;
	else
		wall_x = inf->view.pos.y + inf->ray.dist * inf->ray.dir.y;
	wall_x -= floor(wall_x);
	return (wall_x);
}

int		get_textr_x(t_inf *inf, double wall_x)
{
	int textr_x;

	textr_x = (int)(wall_x * (double)inf->curr_textr.width);
	if (inf->ray.hit_side == 1 && inf->ray.dir.y < 0)
		textr_x = inf->curr_textr.width - textr_x - 1;
	else if (inf->ray.hit_side == 0 && inf->ray.dir.x > 0)
		textr_x = inf->curr_textr.width - textr_x - 1;
	return (textr_x);
}

void	set_curr_textr(t_inf *inf)
{
	int		hit_side;
	double	ray_dir_x;
	double	ray_dir_y;

	hit_side = inf->ray.hit_side;
	ray_dir_x = inf->ray.dir.x;
	ray_dir_y = inf->ray.dir.y;
	if (ray_dir_x <= 0 && ray_dir_y <= 0)
		inf->curr_textr = !hit_side ? inf->textr.n_side : inf->textr.w_side;
	else if (ray_dir_x >= 0 && ray_dir_y >= 0)
		inf->curr_textr = !hit_side ? inf->textr.s_side : inf->textr.e_side;
	else if (ray_dir_x >= 0 && ray_dir_y <= 0)
		inf->curr_textr = !hit_side ? inf->textr.s_side : inf->textr.w_side;
	else
		inf->curr_textr = !hit_side ? inf->textr.n_side : inf->textr.e_side;
}

void	draw_wall(t_inf *inf, int screen_x)
{
	int		textr_x;
	double	textr_step;
	double	textr_y_pos;
	int		textr_y;
	int		screen_y;

	set_curr_textr(inf);
	textr_x = get_textr_x(inf, get_wall_x(inf));
	textr_step = 1.0 * inf->curr_textr.height / inf->ray.wall_height;
	textr_y_pos = (inf->ray.wall_start - inf->height / 2 +
					inf->ray.wall_height / 2) * textr_step;
	screen_y = inf->ray.wall_start;
	while (screen_y < inf->ray.wall_end)
	{
		textr_y = (int)textr_y_pos & (inf->curr_textr.height - 1);
		inf->color.tmp = get_color(&inf->curr_textr, textr_x, textr_y);
		put_color(&inf->img, screen_x, screen_y, inf->color.tmp);
		textr_y_pos += textr_step;
		screen_y++;
	}
}

void	draw_color_range(t_inf *inf, int x, int start_y, int end_y)
{
	while (start_y <= end_y)
	{
		put_color(&inf->img, x, start_y, inf->color.tmp);
		start_y++;
	}
}
