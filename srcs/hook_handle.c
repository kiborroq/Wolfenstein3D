/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_handle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@kiborroq.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 23:10:31 by kiborroq          #+#    #+#             */
/*   Updated: 2021/01/15 11:50:34 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/cub3d.h"

int	event_nadling(t_inf *inf)
{
	raycast(inf);
	mlx_put_image_to_window(inf->mlx, inf->win, inf->img.img_ptr, 0, 0);
	if (inf->event.move_forawrd == 1)
		move_for_back(inf, 1.0);
	if (inf->event.move_backword == 1)
		move_for_back(inf, -1.0);
	if (inf->event.move_left == 1)
		move_left_right(inf, 1.0);
	if (inf->event.move_right == 1)
		move_left_right(inf, -1.0);
	if (inf->event.turn_left == 1)
		turn_left_right(inf, inf->turn.sin_pozotive, inf->turn.cos_pozitive);
	if (inf->event.turn_right == 1)
		turn_left_right(inf, inf->turn.sin_negative, inf->turn.cos_negative);
	return (0);
}

int	button_release(int event_button, t_inf *inf)
{
	if (event_button == MOVE_FORWARD)
		inf->event.move_forawrd = 0;
	else if (event_button == MOVE_BACKWARD)
		inf->event.move_backword = 0;
	else if (event_button == MOVE_LEFT)
		inf->event.move_left = 0;
	else if (event_button == MOVE_RIGHT)
		inf->event.move_right = 0;
	else if (event_button == TURN_LEFT)
		inf->event.turn_left = 0;
	else if (event_button == TURN_RIGHT)
		inf->event.turn_right = 0;
	return (0);
}

int	button_press(int event_button, t_inf *inf)
{
	if (event_button == MOVE_FORWARD)
		inf->event.move_forawrd = 1;
	else if (event_button == MOVE_BACKWARD)
		inf->event.move_backword = 1;
	else if (event_button == MOVE_LEFT)
		inf->event.move_left = 1;
	else if (event_button == MOVE_RIGHT)
		inf->event.move_right = 1;
	else if (event_button == TURN_LEFT)
		inf->event.turn_left = 1;
	else if (event_button == TURN_RIGHT)
		inf->event.turn_right = 1;
	else if (event_button == EXIT)
		close_game(inf);
	return (0);
}
