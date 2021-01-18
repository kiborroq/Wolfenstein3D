/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@kiborroq.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 23:10:31 by kiborroq          #+#    #+#             */
/*   Updated: 2021/01/15 11:51:11 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/cub3d.h"

int	check_map(t_inf *inf)
{
	int	status;
	int i;
	int	num_pos;

	status = OK;
	i = 0;
	num_pos = 0;
	while (status == OK && i < inf->map_height - 1)
	{
		status = check_mapline(inf->map, i, inf->map_width, &num_pos);
		if (num_pos > 1)
			status = KO;
		i++;
	}
	if (num_pos == 0)
		status = KO;
	if (status == KO)
		inf->num_line = inf->start_map + i - 1;
	return (status);
}

int	maplst_to_table(t_inf *inf)
{
	int status;
	int **map;
	int i;

	status = OK;
	if (!(map = (int **)ft_calloc(inf->map_height, sizeof(int *))))
		status = KO;
	i = inf->map_height - 1;
	while (status == OK && i >= 0)
	{
		if (!(map[i] = (int *)malloc(inf->map_width * sizeof(int))))
			status = KO;
		else
			fill_line(map[i], inf->map_width, ft_get_next_contnt(inf->maplst));
		i--;
	}
	if (status == KO)
	{
		ft_free_arr((void **)map, inf->map_height);
		inf->message = NOT_ENOUGH_MEMORY;
	}
	if (status == OK)
		inf->map = map;
	ft_lstclear(&inf->maplst, free);
	return (status);
}

int	add_remain_maplines(int fd, t_inf *inf)
{
	int		status;

	status = OK;
	while (status == OK && (status = get_next_line_wrap(fd, inf)) > 0)
	{
		if ((status = add_mapline_wrap(inf, OTHER_LINE)) != 0)
			ft_freeptr((void **)&inf->line);
	}
	if (status == 0)
	{
		status = add_mapline_wrap(inf, FIRST_LAST_LINE);
		if (status == 0)
			status = ft_isemptyline(inf->line) ? OK : KO;
		if (status != KO && !ismap(inf->maplst->content, FIRST_LAST_LINE))
		{
			inf->num_line--;
			status = KO;
		}
	}
	ft_freeptr((void **)&inf->line);
	if (status == OK)
		status = skip_empty_lines(fd, inf);
	if (status == KO)
		ft_lstclear(&inf->maplst, free);
	return (status);
}

int	add_first_mapline(int fd, t_inf *inf)
{
	int		status;

	status = 0;
	while (status == 0 && (status = get_next_line_wrap(fd, inf)) > 0)
	{
		if ((status = add_mapline_wrap(inf, FIRST_LAST_LINE)) == OK)
			inf->start_map = inf->num_line;
		if (status == 0 && !ft_isemptyline(inf->line))
			status = KO;
		ft_freeptr((void **)&inf->line);
	}
	ft_freeptr((void **)&inf->line);
	return (status);
}
