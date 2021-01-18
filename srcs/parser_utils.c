/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@kiborroq.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 23:10:31 by kiborroq          #+#    #+#             */
/*   Updated: 2021/01/15 11:51:16 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/cub3d.h"

int	skip_empty_lines(int fd, t_inf *inf)
{
	int status;

	status = OK;
	while (status == OK && (status = get_next_line_wrap(fd, inf)) > 0)
	{
		status = ft_isemptyline(inf->line) ? OK : KO;
		ft_freeptr((void **)&inf->line);
	}
	if (status == 0)
		status = ft_isemptyline(inf->line) ? OK : KO;
	ft_freeptr((void **)&inf->line);
	return (status);
}

int	get_next_line_wrap(int fd, t_inf *inf)
{
	int status;

	inf->num_line++;
	if ((status = get_next_line(fd, &inf->line)) == -1)
		inf->message = READEING_ERROR;
	return (status);
}
