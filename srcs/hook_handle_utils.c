/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_handle_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@kiborroq.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 23:10:31 by kiborroq          #+#    #+#             */
/*   Updated: 2021/01/15 11:50:29 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/cub3d.h"

void	move_for_back(t_inf *inf, double dir)
{
	double delta;

	delta = inf->view.dir.x * inf->speed.move * dir;
	if (!inf->map[(int)(inf->view.pos.x + delta * 2)][(int)inf->view.pos.y])
	{
		inf->view.pos.x += delta;
	}
	delta = inf->view.dir.y * inf->speed.move * dir;
	if (!inf->map[(int)inf->view.pos.x][(int)(inf->view.pos.y + delta * 2)])
	{
		inf->view.pos.y += delta;
	}
}

void	move_left_right(t_inf *inf, double dir)
{
	double delta;

	delta = inf->view.dir.y * inf->speed.move * dir;
	if (!inf->map[(int)(inf->view.pos.x - delta * 2)][(int)inf->view.pos.y])
	{
		inf->view.pos.x -= delta;
	}
	delta = inf->view.dir.x * inf->speed.move * dir;
	if (!inf->map[(int)inf->view.pos.x][(int)(inf->view.pos.y + delta * 2)])
	{
		inf->view.pos.y += delta;
	}
}

void	turn_left_right(t_inf *inf, double turn_sin, double turn_cos)
{
	double old_x;

	old_x = inf->view.dir.x;
	inf->view.dir.x = inf->view.dir.x * turn_cos -
						inf->view.dir.y * turn_sin;
	inf->view.dir.y = inf->view.dir.y * turn_cos +
						old_x * turn_sin;
	old_x = inf->view.cam.x;
	inf->view.cam.x = inf->view.cam.x * turn_cos -
							inf->view.cam.y * turn_sin;
	inf->view.cam.y = inf->view.cam.y * turn_cos +
							old_x * turn_sin;
}
