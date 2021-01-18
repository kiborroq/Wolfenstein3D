/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@kiborroq.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 23:10:31 by kiborroq          #+#    #+#             */
/*   Updated: 2021/01/15 12:36:15 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/cub3d.h"

int	init_textr_rgb_resltn(char *line, t_inf *inf)
{
	int		status;
	char	*elem;

	status = 0;
	if (!(elem = ft_strtrim(line, SPACES)))
	{
		status = KO;
		inf->message = NOT_ENOUGH_MEMORY;
	}
	if (status == 0)
		status = init_textr(inf, elem);
	if (status == 0)
		status = init_rgb(inf, elem);
	if (status == 0)
		status = init_resltn(inf, elem);
	if (status == 0)
		status = check_elem(inf, elem);
	ft_freeptr((void **)&elem);
	return (status);
}

int	config_parser(int fd, t_inf *inf)
{
	int		n;
	int		status;

	n = 0;
	status = 0;
	while (status != KO && n < NUM_ELEMS && get_next_line_wrap(fd, inf) > 0)
	{
		if ((status = init_textr_rgb_resltn(inf->line, inf)) == OK)
			n++;
		ft_freeptr((void **)&inf->line);
	}
	ft_freeptr((void **)&inf->line);
	if (status != KO && n < NUM_ELEMS)
	{
		status = KO;
		if (inf->message == 0)
			inf->message = NOT_ENOUGH_CONFIG;
	}
	return (status);
}

int	map_parser(int fd, t_inf *inf)
{
	int status;

	status = add_first_mapline(fd, inf);
	if (status == OK)
		status = add_remain_maplines(fd, inf);
	if (status == OK)
		status = maplst_to_table(inf);
	if (status == OK)
		status = check_map(inf);
	if (status == KO && inf->message == 0)
		inf->message = INVALID_MAP;
	return (status);
}

int	config_map_parser(t_inf *inf, char *map_path)
{
	int		fd;
	int		status;

	if ((fd = open(map_path, O_RDONLY)) == KO)
	{
		inf->message = NO_FILE_DIR;
		return (KO);
	}
	status = config_parser(fd, inf);
	if (status != KO)
		status = map_parser(fd, inf);
	close(fd);
	return (status);
}
