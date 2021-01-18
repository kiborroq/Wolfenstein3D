/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@kiborroq.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 23:10:31 by kiborroq          #+#    #+#             */
/*   Updated: 2021/01/15 13:47:36 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/cub3d.h"

void	destroy_mlx_elems(t_inf *inf)
{
	if (inf->textr.e_side.img_ptr)
		mlx_destroy_image(inf->mlx, inf->textr.e_side.img_ptr);
	if (inf->textr.w_side.img_ptr)
		mlx_destroy_image(inf->mlx, inf->textr.w_side.img_ptr);
	if (inf->textr.s_side.img_ptr)
		mlx_destroy_image(inf->mlx, inf->textr.s_side.img_ptr);
	if (inf->textr.n_side.img_ptr)
		mlx_destroy_image(inf->mlx, inf->textr.n_side.img_ptr);
	if (inf->textr.spr.img_ptr)
		mlx_destroy_image(inf->mlx, inf->textr.spr.img_ptr);
	if (inf->img.img_ptr)
		mlx_destroy_image(inf->mlx, inf->img.img_ptr);
	if (inf->win)
		mlx_destroy_window(inf->mlx, inf->win);
	if (inf->mlx)
		free(inf->mlx);
}

void	print_error_message(t_inf *inf)
{
	if (!inf)
		ft_putendl_fd(NOT_ENOUGH_MEMORY, 1);
	else if (inf->message)
	{
		if (inf->num_line != 0 &&
			ft_strcmp(inf->message, NOT_ENOUGH_MEMORY) &&
			ft_strcmp(inf->message, READEING_ERROR) &&
			ft_strcmp(inf->message, SAVE_ERROR))
		{
			ft_putstr_fd("line ", 1);
			ft_putnbr_fd(inf->num_line, 1);
			ft_putstr_fd(" -> ", 1);
		}
		ft_putendl_fd(inf->message, 1);
	}
}
