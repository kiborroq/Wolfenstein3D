/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@kiborroq.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 23:10:31 by kiborroq          #+#    #+#             */
/*   Updated: 2021/01/15 11:50:46 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/cub3d.h"

t_view	set_dir_cam(double d_x, double d_y, double c_x, double c_y)
{
	t_view view;

	view.dir.x = d_x;
	view.dir.y = d_y;
	view.cam.x = c_x;
	view.cam.y = c_y;
	return (view);
}

void	set_pos_dir_cam(t_inf *inf, double p_x, double p_y, char dir)
{
	t_view	view;

	if (dir == 'W')
		view = set_dir_cam(0.0, -1.0, -0.66, 0.0);
	else if (dir == 'E')
		view = set_dir_cam(0.0, 1.0, 0.66, 0.0);
	else if (dir == 'N')
		view = set_dir_cam(-1.0, 0.0, 0.0, 0.66);
	else if (dir == 'S')
		view = set_dir_cam(1.0, 0.0, 0.0, -0.66);
	view.pos.x = p_x;
	view.pos.y = p_y;
	inf->view = view;
}
