/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@kiborroq.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/11 10:59:31 by kiborroq          #+#    #+#             */
/*   Updated: 2021/01/15 15:15:51 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/cub3d.h"

int	raycast(t_inf *inf)
{
	int screen_x;

	screen_x = 0;
	while (screen_x < inf->width)
	{
		init_ray(inf, screen_x);
		calculate_step(inf);
		calculate_dist_to_wall(inf, screen_x);
		calculate_wall_range(inf);
		draw_stripe(inf, screen_x);
		screen_x++;
	}
	draw_sprs(inf);
	return (1);
}

int	close_game(t_inf *inf)
{
	print_error_message(inf);
	if (inf)
	{
		destroy_mlx_elems(inf);
		ft_free_arr((void **)inf->map, inf->map_height);
		ft_freeptr((void **)&inf->sprs);
		ft_freeptr((void **)&inf->dist_buf);
		ft_freeptr((void **)&inf);
	}
	exit(1);
	return (0);
}

int	save_screen(t_inf *inf)
{
	int			fd;
	t_bitmap	bmp;

	raycast(inf);
	if ((fd = open(SCREENSHOT, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU)) == KO)
	{
		inf->message = SAVE_ERROR;
		return (KO);
	}
	init_bmp(&bmp, inf);
	write_bmp(bmp, inf, fd);
	close(fd);
	return (OK);
}

int	init_game(char *map_path, t_inf **inf)
{
	int status;

	status = init_inf_mlx(inf);
	status = status == OK ? config_map_parser(*inf, map_path) : KO;
	status = status == OK ? init_img(*inf) : KO;
	status = status == OK ? init_sprs(*inf) : KO;
	if (status == OK)
	{
		init_view(*inf);
		init_speed(*inf);
	}
	else if (*inf != 0 && (*inf)->message == 0)
		(*inf)->message = NOT_ENOUGH_MEMORY;
	return (status);
}

int	main(int argc, char **argv)
{
	t_inf	*inf;

	if (argc < 2 || argc > 3 || ft_strrncmp(argv[1], EXTENTN, 4) ||
		(argc == 3 && ft_strcmp(argv[2], SAVE_FLAG)))
	{
		ft_putendl_fd(ARG_ERROR, 1);
		return (0);
	}
	if (init_game(argv[1], &inf) == KO)
		close_game(inf);
	if (argc == 3)
	{
		save_screen(inf);
		close_game(inf);
	}
	if (!(inf->win = mlx_new_window(inf->mlx, inf->width, inf->height, NAME)))
		close_game(inf);
	mlx_hook(inf->win, 17, 1L << 17, close_game, inf);
	mlx_hook(inf->win, 2, 1L << 0, button_press, inf);
	mlx_loop_hook(inf->mlx, event_nadling, inf);
	mlx_hook(inf->win, 3, 1L << 1, button_release, inf);
	mlx_loop(inf->mlx);
	return (0);
}
