/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@kiborroq.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 23:10:31 by kiborroq          #+#    #+#             */
/*   Updated: 2021/01/15 11:51:44 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/cub3d.h"

void	write_data(t_bitmap bmp, int sl, int fd)
{
	int y;

	y = bmp.i_h.height - 1;
	while (y >= 0)
	{
		write(fd, &bmp.data[y * sl / 4], sl);
		y--;
	}
}

void	write_bmp(t_bitmap bmp, t_inf *inf, int fd)
{
	write(fd, bmp.h.sign, 2);
	write(fd, &bmp.h.file_size, 4);
	write(fd, &bmp.h.res_byte, 2);
	write(fd, &bmp.h.res_byte, 2);
	write(fd, &bmp.h.offset, 4);
	write(fd, &bmp.i_h.size, 4);
	write(fd, &bmp.i_h.width, 4);
	write(fd, &bmp.i_h.height, 4);
	write(fd, &bmp.i_h.planes, 2);
	write(fd, &bmp.i_h.bpp, 2);
	write(fd, &bmp.i_h.compres, 4);
	write(fd, &bmp.i_h.img_size, 4);
	write(fd, &bmp.i_h.hor_resol, 4);
	write(fd, &bmp.i_h.ver_resol, 4);
	write(fd, &bmp.i_h.color_used, 4);
	write(fd, &bmp.i_h.color_important, 4);
	write_data(bmp, inf->img.sl, fd);
}

void	init_bmp(t_bitmap *bmp, t_inf *inf)
{
	bmp->i_h.size = 40;
	bmp->i_h.width = inf->img.width;
	bmp->i_h.height = inf->img.height;
	bmp->i_h.planes = 1;
	bmp->i_h.bpp = inf->img.bpp;
	bmp->i_h.compres = 0;
	bmp->i_h.img_size = 4 * inf->img.width * inf->img.height;
	bmp->i_h.hor_resol = inf->img.width;
	bmp->i_h.ver_resol = inf->img.height;
	bmp->i_h.color_used = 0;
	bmp->i_h.color_important = 0;
	bmp->h.sign = "BM";
	bmp->h.file_size = 14 + bmp->i_h.size + bmp->i_h.img_size;
	bmp->h.res_byte = 0;
	bmp->h.offset = 14 + bmp->i_h.size;
	bmp->data = inf->img.data;
}
