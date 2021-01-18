/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@kiborroq.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 23:10:31 by kiborroq          #+#    #+#             */
/*   Updated: 2021/01/15 11:50:10 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/cub3d.h"

void	put_color(t_image *img, int x, int y, int color)
{
	img->data[y * img->sl / 4 + x] = color;
}

int		get_color(t_image *img, int x, int y)
{
	return (img->data[y * img->sl / 4 + x]);
}

void	swap_sprs(t_spr *a, t_spr *b)
{
	t_spr tmp;

	tmp.dist = a->dist;
	tmp.abs_pos.x = a->abs_pos.x;
	tmp.abs_pos.y = a->abs_pos.y;
	a->dist = b->dist;
	a->abs_pos.x = b->abs_pos.x;
	a->abs_pos.y = b->abs_pos.y;
	b->dist = tmp.dist;
	b->abs_pos.x = tmp.abs_pos.x;
	b->abs_pos.y = tmp.abs_pos.y;
}

void	sort_sprs(t_spr *sprs, int num_sprs)
{
	int i;
	int j;

	i = 0;
	while (i + 1 < num_sprs)
	{
		j = 0;
		while (j + 1 < num_sprs)
		{
			if (sprs[j].dist < sprs[j + 1].dist)
				swap_sprs(&sprs[j], &sprs[j + 1]);
			j++;
		}
		i++;
	}
}
