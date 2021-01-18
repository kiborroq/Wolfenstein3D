/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parser_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@kiborroq.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 23:10:31 by kiborroq          #+#    #+#             */
/*   Updated: 2021/01/15 11:51:06 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/cub3d.h"

int		ismap(char *elem, char *set)
{
	int not_only_spaces;

	not_only_spaces = 0;
	while (*elem)
	{
		if (!ft_strchr(set, *elem))
			return (0);
		if (not_only_spaces == 0 && *elem != ' ')
			not_only_spaces = 1;
		elem++;
	}
	return (not_only_spaces);
}

int		add_mapline(t_list **maplst, char *line)
{
	t_list *curr;

	if (!(curr = ft_lstnew(line)))
		return (KO);
	ft_lstadd_front(maplst, curr);
	return (OK);
}

int		add_mapline_wrap(t_inf *inf, char *set)
{
	int status;

	if ((status = ismap(inf->line, set)) == OK)
	{
		if ((status = add_mapline(&inf->maplst, inf->line)) == OK)
		{
			inf->map_height++;
			inf->map_width = ft_max(inf->map_width, ft_strlen(inf->line));
			inf->line = 0;
		}
		else
		{
			inf->message = NOT_ENOUGH_MEMORY;
			ft_lstclear(&inf->maplst, free);
		}
	}
	return (status);
}

void	fill_line(int *line, int line_len, char *src)
{
	int j;
	int src_len;

	j = 0;
	src_len = ft_strlen(src);
	while (j < line_len)
	{
		if (j < src_len)
			line[j] = src[j] == ' ' ? -1 : src[j] - '0';
		else
			line[j] = -1;
		j++;
	}
}

int		check_mapline(int **map, int i, int width, int *num_pos)
{
	int j;

	if ((map[i][0] != WALL && map[i][0] != SPACE) ||
		(map[i][width - 1] != WALL && map[i][width - 1] != SPACE))
		return (KO);
	j = 0;
	while (j < width - 1)
	{
		if (map[i][j] == SPACE)
		{
			if ((map[i][j + 1] != WALL && map[i][j + 1] != SPACE) ||
				(map[i + 1][j] != WALL && map[i + 1][j] != SPACE))
				return (KO);
		}
		else if (map[i][j] != WALL)
		{
			if (map[i][j + 1] == SPACE || map[i + 1][j] == SPACE)
				return (KO);
		}
		if (ft_strchr(START_POS, map[i][j] + '0'))
			(*num_pos)++;
		j++;
	}
	return (OK);
}
