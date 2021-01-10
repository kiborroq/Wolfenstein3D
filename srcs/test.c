/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/11 10:59:31 by kiborroq          #+#    #+#             */
/*   Updated: 2021/01/10 23:45:09 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define OK 1
#define KO -1

#define NUM_ELEMS 8
#define SPACES " \n\t\v\r\f"

#define NOT_ENOUGH_MEMORY "Not enough memory for allocation"
#define NOT_ENOUGH_CONFIG "Map start but not all config elems are defined"
#define COLOR_ERROR "Error in color elem"
#define TEXTURE_ERROR "Error in texture or sprite elem"
#define RESOLUTION_ERROR "Error in resolution elem"
#define UNKNOWN_ELEM "Unknown config elem"

#define MAP_HEIGHT 24
#define MAP_WIDTH 24
#define textr_HEIGHT 64
#define textr_WIDTH 64

#define WALL 1
#define SPRITE 2

#define COLORLESS -16777216

#define EXIT 65307
#define MOVE_FORWARD 119
#define MOVE_BACKWARD 115
#define MOVE_LEFT 97
#define MOVE_RIGHT 100
#define TURN_LEFT 65361
#define TURN_RIGHT 65363 
#define MOVE_SPEED 0.0455
#define TURN_SPEED 0.03276

#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "math.h"
#include "../minilibx/mlx.h"
#include "../libft/libft.h"
#include "fcntl.h"
#include "../get_next_line/get_next_line.h"

typedef struct s_color
{
    int ceil;
    int floor;
}              t_color;

typedef struct s_header
{
	char *sign;
	int file_size;
	int res_byte;
    int offset;
}				t_header;

typedef struct s_info_header
{
	int size;
	int width;
	int height;
	int planes;
	int bpp;
	int compres;
	int img_size;
	int hor_resol;
	int ver_resol;
	int color_used;
	int color_important;
}				t_info_header;


typedef struct	s_bitmap
{
	t_header h;
	t_info_header i_h;
	int	*data;
}				t_bitmap;


typedef struct  s_point_d
{
    double x;
    double y;
}               t_point_d;

typedef struct  s_point_i
{
    int x;
    int y;
}               t_point_i;

typedef struct  s_spr
{
    t_point_d   abs_pos;
    double      dist;
	t_point_d	transform;
	int			screen_x;
	t_point_i	start;
	t_point_i	end;
	int			width;
	int			height;
}               t_spr;

typedef struct      s_view
{
    t_point_d  pos;
    t_point_d  dir;
    t_point_d  cam_plane;
}                   t_view;

typedef struct s_turn_sin_cos
{
    double sin_pozotive;
    double sin_negative;
    double cos_pozitive;
    double cos_negative;
}               t_turn_sin_cos;

typedef struct s_event
{
    int move_forawrd;
    int move_backword;
    int move_left;
    int move_right;
    int turn_left;
    int turn_right;
}               t_event;

typedef struct      s_ray
{
    t_point_d  		dir;
    t_point_i       pos;
    t_point_d       cur_dist;
    t_point_d       delt_dist;
    t_point_i       step;
    double          dist;
    int             hit_side;
    int             wall_start;
    int             wall_end;
    int             wall_height;
}                   t_ray;

typedef struct			s_image
{
	int					sl;
	int					bpp;
	int					endian;
	void				*img_ptr;
	int	                *data;
    int                 width;
    int                 height;
}						t_image;

typedef struct s_textr
{
    t_image n_side;
    t_image s_side;
    t_image e_side;
    t_image w_side;
    t_image spr;
}               t_textr;

typedef struct  s_inf
{
    t_view          view;
    t_ray           ray;
    t_turn_sin_cos  turn;
    void            *mlx;
    void            *win;
    t_image         img;
    int             **map;
    t_event         event;
    t_textr         textr;
    t_image         test;
    t_color         color;
    t_image         curr_textr;
    t_spr           *sprs;
    int             num_sprs;
	double			*dist_buf;
	int				map_line;
	char			*message;
	int				width;
	int				height;
}                   t_inf;

void    put_color(t_image *img, int x, int y, int color)
{
    img->data[y * img->sl / 4 + x] = color;
}

int     get_color(t_image *img, int x, int y)
{
    return (img->data[y * img->sl / 4 + x]);
}

void set_curr_textr(t_inf *inf)
{
    int hit_side;
    double ray_dir_x;
    double ray_dir_y;

    hit_side = inf->ray.hit_side;
    ray_dir_x = inf->ray.dir.x;
    ray_dir_y = inf->ray.dir.y;
    if (ray_dir_x <= 0 && ray_dir_y <= 0)
        inf->curr_textr = !hit_side ? inf->textr.n_side : inf->textr.w_side;
    else if (ray_dir_x >= 0 && ray_dir_y >= 0)
        inf->curr_textr = !hit_side ? inf->textr.s_side : inf->textr.e_side;
    else if (ray_dir_x >= 0 && ray_dir_y <= 0)
        inf->curr_textr = !hit_side ? inf->textr.s_side : inf->textr.w_side;       
    else
        inf->curr_textr = !hit_side ? inf->textr.n_side : inf->textr.e_side;
    
}

double get_wall_x(t_inf *inf)
{
    double wall_x;

    if (inf->ray.hit_side == 1)
        wall_x = inf->view.pos.x + inf->ray.dist * inf->ray.dir.x;
    else
        wall_x = inf->view.pos.y + inf->ray.dist * inf->ray.dir.y;
    wall_x -= floor(wall_x);
    return (wall_x);
}

int get_textr_x(t_inf *inf, double wall_x)
{
    int textr_x;

    textr_x = (int)(wall_x * (double)inf->curr_textr.width);
    if (inf->ray.hit_side == 1 && inf->ray.dir.y < 0)
        textr_x = inf->curr_textr.width - textr_x - 1;
    else if (inf->ray.hit_side == 0 && inf->ray.dir.x > 0)
        textr_x = inf->curr_textr.width - textr_x - 1;
    return (textr_x);
}

void draw_wall(t_inf *inf, int screen_x)
{
    double          wall_x;
    int             textr_x;
    double          textr_step;
    double          textr_y_pos;
    int             textr_y;
    int             screen_y;
    int             color;

    set_curr_textr(inf);
    wall_x = get_wall_x(inf);
    textr_x = get_textr_x(inf, wall_x);
    textr_step = 1.0 * inf->curr_textr.height / inf->ray.wall_height;
    textr_y_pos = (inf->ray.wall_start - inf->height / 2 + inf->ray.wall_height / 2) * textr_step;
    screen_y = inf->ray.wall_start;
    while (screen_y < inf->ray.wall_end)
    {
        textr_y = (int)textr_y_pos & (inf->curr_textr.height - 1);
        color = get_color(&inf->curr_textr, textr_x, textr_y);
        put_color(&inf->img, screen_x, screen_y, color);
        textr_y_pos += textr_step;
        screen_y++;
    }
}

void init_ray(t_inf *inf, int screen_x)
{
    double pos_on_plane;

    pos_on_plane = 2 * screen_x / (double)inf->width - 1;
    inf->ray.dir.x = inf->view.dir.x + pos_on_plane * inf->view.cam_plane.x;
    inf->ray.dir.y = inf->view.dir.y + pos_on_plane * inf->view.cam_plane.y;
    inf->ray.pos.x = (int)inf->view.pos.x;
    inf->ray.pos.y = (int)inf->view.pos.y;
    if (inf->ray.dir.y != 0)
        inf->ray.delt_dist.x = inf->ray.dir.x == 0 ? 1 : fabs(1 / inf->ray.dir.x);
    else
        inf->ray.delt_dist.x = 0;
    if (inf->ray.dir.x != 0)
        inf->ray.delt_dist.y = inf->ray.dir.y == 0 ? 1 : fabs(1 / inf->ray.dir.y);
    else
        inf->ray.delt_dist.y = 0;
}

void calculate_step(t_inf *inf)
{
    if (inf->ray.dir.x < 0)
    {
        inf->ray.step.x = -1;
        inf->ray.cur_dist.x = (inf->view.pos.x - inf->ray.pos.x) *inf->ray.delt_dist.x;
    }
    else
    {
        inf->ray.step.x = 1;
        inf->ray.cur_dist.x = (inf->ray.pos.x + 1.0 - inf->view.pos.x) * inf->ray.delt_dist.x;
    }
    if (inf->ray.dir.y < 0)
    {
        inf->ray.step.y = -1;
        inf->ray.cur_dist.y = (inf->view.pos.y - inf->ray.pos.y) *  inf->ray.delt_dist.y;
    }
    else
    {
        inf->ray.step.y = 1;
        inf->ray.cur_dist.y = (inf->ray.pos.y + 1.0 - inf->view.pos.y) *  inf->ray.delt_dist.y;
    }
}

void reset_sprs(t_spr *sprs, int *num_actual_sprs)
{
	int i;

	i = 0;
	while (i < *num_actual_sprs)
	{
		sprs[i].dist = -1;	
		i++;
	}
	*num_actual_sprs = 0;
}

void count_dist_to_spr(t_inf *inf)
{
	int	spr_x;
	int	spr_y;
	int view_x;
	int view_y;
	int	i;
	int	delta_x;
	int delta_y;


	view_x = inf->view.pos.x;
	view_y = inf->view.pos.y;
	i = 0;
	while (i < inf->num_sprs)
	{
		spr_x = inf->sprs[i].abs_pos.x;
		spr_y = inf->sprs[i].abs_pos.y;
		delta_x = abs(spr_x - view_x);
		delta_y = abs(spr_y - view_y);
		inf->sprs[i].dist = sqrt(delta_y * delta_y + delta_x * delta_x);
		i++;
	}
}

void calculate_dist_to_wall(t_inf *inf, int screen_x)
{
    while (1)
    {
        if (inf->ray.cur_dist.x < inf->ray.cur_dist.y)
        {
            inf->ray.cur_dist.x += inf->ray.delt_dist.x;
            inf->ray.pos.x += inf->ray.step.x;
            inf->ray.hit_side = 0;
        }
        else
        {
            inf->ray.cur_dist.y += inf->ray.delt_dist.y;
            inf->ray.pos.y += inf->ray.step.y;
            inf->ray.hit_side = 1;
        }
        if (inf->map[inf->ray.pos.x][inf->ray.pos.y] == WALL)
            break;
    }
    if (inf->ray.hit_side == 0)
        inf->ray.dist = (inf->ray.pos.x - inf->view.pos.x +
                        (1 - inf->ray.step.x) / 2) / inf->ray.dir.x;
    else
	{
        inf->ray.dist = (inf->ray.pos.y - inf->view.pos.y +
                        (1 - inf->ray.step.y) / 2) / inf->ray.dir.y;
	}
	inf->dist_buf[screen_x] = inf->ray.dist;
}

void calculate_wall_range(t_inf *inf)
{
    int height;
    int start;
    int end;

    height = (int)(inf->height / inf->ray.dist);
    if ((start = -height / 2 + inf->height / 2) < 0)
        start = 0;
    if ((end = height / 2 + inf->height / 2) >= inf->height)
        end = inf->height - 1;
    inf->ray.wall_height = height;
    inf->ray.wall_start = start;
    inf->ray.wall_end = end;
}

void draw_color_range(t_inf *inf, int x, int start_y, int end_y, int color)
{
    while (start_y <= end_y)
    {
        put_color(&inf->img, x, start_y, color);
        start_y++;
    }
}

void draw_stripe(t_inf *inf, int screen_x)
{
    draw_color_range(inf, screen_x, 0, inf->ray.wall_start - 1, inf->color.ceil);
    draw_wall(inf, screen_x);
    draw_color_range(inf, screen_x, inf->ray.wall_end, inf->height - 1, inf->color.floor);
}

void swap_sprs(t_spr *a, t_spr *b)
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

void print_sprs(t_spr *sprs, int num)
{
	int i;
	
	i = 0;
	while (i < num)
	{
		ft_putnbr_fd((sprs + i)->abs_pos.x, 1);
		ft_putstr_fd(" ", 1);
		ft_putnbr_fd(sprs[i].abs_pos.y, 1);
		ft_putstr_fd(" ", 1);
		ft_putnbr_fd((int)sprs[i].dist, 1);
		ft_putstr_fd("\n", 1);
		i++;
	}
	ft_putstr_fd("\n\n", 1);
}

void sort_sprs(t_spr *sprs, int num_sprs)
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

void transform_spr(t_spr *spr, t_inf *inf)
{
	t_point_d rel_pos;
	double inv_det;
	
	rel_pos.x = spr->abs_pos.x - inf->view.pos.x;
	rel_pos.y = spr->abs_pos.y - inf->view.pos.y;
	inv_det = 1.0 / (inf->view.cam_plane.x * inf->view.dir.y -
					inf->view.cam_plane.y * inf->view.dir.x);
	spr->transform.x = inv_det * (inf->view.dir.y * rel_pos.x -
								inf->view.dir.x * rel_pos.y);
	spr->transform.y = inv_det * (-inf->view.cam_plane.y * rel_pos.x +
								inf->view.cam_plane.x * rel_pos.y);
	spr->screen_x = (int)((inf->width / 2) *
					(1 + spr->transform.x / spr->transform.y));
}

void calculate_spr_range(t_spr *spr, int width, int height)
{
	spr->height = abs((int)(height / spr->transform.y));
	if ((spr->start.y = -spr->height / 2 + height / 2) < 0)
		spr->start.y = 0;
	if ((spr->end.y = spr->height / 2 + height / 2) >= height)
		spr->end.y = height - 1;
	spr->width = spr->height;
	if ((spr->start.x = -spr->width / 2 + spr->screen_x) < 0)
		spr->start.x = 0;
	if ((spr->end.x = spr->width / 2 + spr->screen_x) >= width)
		spr->end.x = width - 1;	
}

void draw_curr_spr(t_spr *spr, t_image *spr_img, t_inf *inf)
{
	int x;
	int y;
	double textr_step;
	int textr_x;
	int textr_y;
	int color;

	x = spr->start.x;
    textr_step = 1.0 * spr_img->height / spr->height;
	while (x < spr->end.x)
	{
		textr_x = (int)(spr_img->sl * (x - (-spr->width / 2 + spr->screen_x)) *
						spr_img->width / spr->width) / spr_img->sl;
		y = spr->start.y;
        if (spr->transform.y > 0 && x > 0 && x < inf->width && spr->transform.y < inf->dist_buf[x])
        {
            while (y < spr->end.y)
		    {
		    	textr_y = (y - inf->height / 2 + spr->height / 2) * textr_step;
		    	color = get_color(spr_img, textr_x, textr_y);
		    	if (color != COLORLESS)
		    		put_color(&inf->img, x, y, color);
		    	y++;
		    }
        }
		x++;
	}
}

void draw_sprs(t_inf *inf)
{
	int i;

	i = 0;
	count_dist_to_spr(inf);
	sort_sprs(inf->sprs, inf->num_sprs);
	while (i < inf->num_sprs)
	{
		transform_spr(inf->sprs + i, inf);
		calculate_spr_range(inf->sprs + i, inf->width, inf->height);
		draw_curr_spr(inf->sprs + i, &inf->textr.spr, inf);
		i++;
	}
}

int raycast(t_inf *inf)
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

int **fill_map(int map[MAP_HEIGHT][MAP_WIDTH])
{
    int **new_map;

    new_map = (int **)malloc(sizeof(int *) * MAP_HEIGHT);
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        new_map[i] = (int *)malloc(sizeof(int) * MAP_WIDTH);
        for (int j = 0; j < MAP_WIDTH; j++)
            new_map[i][j] = map[i][j];        
    }
    return (new_map);
}

void move_for_back(t_inf *inf, double dir)
{
    double delta;

    delta = inf->view.dir.x * MOVE_SPEED * dir;
    if (inf->map[(int)(inf->view.pos.x + delta * 2)][(int)inf->view.pos.y] == 0)
    {
        inf->view.pos.x += delta;
    }
    delta = inf->view.dir.y * MOVE_SPEED * dir;
    if (inf->map[(int)inf->view.pos.x][(int)(inf->view.pos.y + delta * 2)] == 0)
    {
        inf->view.pos.y += delta;
    }
}

void move_left_right(t_inf *inf, double dir)
{
    double delta;

    delta = inf->view.dir.y * MOVE_SPEED * dir;
    if (inf->map[(int)(inf->view.pos.x - delta * 2)][(int)inf->view.pos.y] == 0)
    {
        inf->view.pos.x -= delta;
    }
    delta = inf->view.dir.x * MOVE_SPEED * dir;
    if (inf->map[(int)inf->view.pos.x][(int)(inf->view.pos.y + delta * 2)] == 0)
    {
        inf->view.pos.y += delta;
    }
}

void turn_left_right(t_inf *inf, double turn_sin, double turn_cos)
{
    double old_x;

    old_x = inf->view.dir.x;
    inf->view.dir.x =       inf->view.dir.x * turn_cos -
                            inf->view.dir.y * turn_sin;
    inf->view.dir.y =       inf->view.dir.y * turn_cos +
                            old_x * turn_sin;
    old_x = inf->view.cam_plane.x;
    inf->view.cam_plane.x = inf->view.cam_plane.x * turn_cos -
                            inf->view.cam_plane.y * turn_sin;
    inf->view.cam_plane.y = inf->view.cam_plane.y * turn_cos +
                            old_x * turn_sin;
}

void free_array(void **array, int height)
{
	int i;

	i = 0;
	if (array)
	{
		while (i < height)
		{
			if (array[i])
				free(array[i]);
			i++;
		}
		free(array);
	}
}

void destroy_mlx_elems(t_inf *inf)
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

void	free_ptrs(void **p1, void **p2, void **p3)
{
	if (p1 && *p1)
	{
		free(*p1);
		*p1 = 0;
	}
	if (p2 && *p2)
	{
		free(*p2);
		*p2 = 0;
	}
	if (p3 && *p3)
	{
		free(*p3);
		*p3 = 0;
	}
}

int close_game(t_inf *inf)
{
    destroy_mlx_elems(inf);
    free_array((void **)inf->map, MAP_WIDTH);
    free_ptrs((void **)&inf->sprs, (void **)&inf->dist_buf, (void **)&inf);
    exit (1);
    return (0);
}

int event_nadling(t_inf *inf)
{
    raycast(inf);
    mlx_put_image_to_window(inf->mlx, inf->win, inf->img.img_ptr, 0 , 0);
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

int button_release(int event_button, t_inf *inf)
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

int button_press(int event_button, t_inf *inf)
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

int num_digits(int **array, int array_height, int array_width, int dig)
{
    int i;
    int j;
    int num;

    i = 0;
    num = 0;
    while (i < array_height)
    {
		j = 0;
        while (j < array_width)
        {
            if (array[i][j] == dig)
                num++;
            j++;
        }
        i++;
    }
    return (num);
}

int init_sprs(t_inf *inf)
{
    int         x;
    int         y;
	int			i;

    inf->num_sprs = num_digits(inf->map, MAP_HEIGHT, MAP_WIDTH, SPRITE);
    if (!(inf->sprs = (t_spr *)malloc(inf->num_sprs * sizeof(t_spr))))
		return (KO);
	x = 0;
	i = 0;
	while (x < MAP_HEIGHT && i < inf->num_sprs)
	{
		y = 0;
		while (y < MAP_WIDTH && i < inf->num_sprs)
        {
            if (inf->map[x][y] == SPRITE)
			{
				inf->sprs[i].abs_pos.x = x + .5;
				inf->sprs[i].abs_pos.y = y + .5;
				inf->sprs[i].dist = .0;
				i++;
			}
            y++;
        }
		x++;
	}
	return (OK);
}

int add_textr(void *mlx, t_image *textr, char *path)
{
    if (!(textr->img_ptr = mlx_xpm_file_to_image(mlx, path, &textr->width, &textr->height)))
    {
        return (KO);
    }
    textr->data = (int *)mlx_get_data_addr(textr->img_ptr,
                                            &textr->bpp,
                                            &textr->sl,
                                            &textr->endian);
    return (OK);
}

void init_bmp(t_bitmap *bmp, t_inf *inf)
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

void write_data(t_bitmap bmp, int sl, int fd)
{
    int y;

    y = bmp.i_h.height - 1;
	while (y >= 0)
	{
        write(fd, &bmp.data[y * sl / 4], sl);
		y--;
	}
}

void write_bmp(t_bitmap bmp, t_inf *inf, int fd)
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

void save_screen(t_inf *inf)
{
	int fd;
	t_bitmap bmp;

	raycast(inf);
	fd = open("screenshot.bmp", O_CREAT | O_RDWR);
    init_bmp(&bmp, inf);
    write_bmp(bmp, inf, fd);
	close(fd);
}

int	isspaces(int ch)
{
    if (ch == 32 || (ch > 8 && ch < 14))
            return (1);
    return (0);
}

char *skip_spaces(char *str)
{
	while (isspaces(*str) && *str)
		str++;
	return (str);
}

int	add_textr_wrap(void *mlx, t_image *textr, char *path, char *elem)
{
	if (path > elem)
		return (add_textr(mlx, textr, path));
	return (KO);
}

int	init_textr(t_inf *inf, char *elem)
{
	int status;
	char *path;

	status = 0;
	path = skip_spaces(elem + 2);
	if (!ft_strncmp(elem, "EA", 2))
		status = add_textr_wrap(inf->mlx, &inf->textr.e_side, path, elem + 2);
	else if (!ft_strncmp(elem, "WE", 2))
		status = add_textr_wrap(inf->mlx, &inf->textr.w_side, path, elem + 2);
	else if (!ft_strncmp(elem, "SO", 2))
		status = add_textr_wrap(inf->mlx, &inf->textr.s_side, path, elem + 2);
	else if (!ft_strncmp(elem, "NO", 2))
		status = add_textr_wrap(inf->mlx, &inf->textr.n_side, path, elem + 2);
	else if (!ft_strncmp(elem, "S", 1))
	{
		path = skip_spaces(elem + 1);
		status = add_textr_wrap(inf->mlx, &inf->textr.spr, path, elem + 1);
	}
	if (status == KO)
		inf->message = TEXTURE_ERROR;
	return (status);
}

char *skip_digits(char *str)
{
	while (ft_isdigit(*str))
		str++;
	return (str);
}

int add_rgb(int *color, char *rgb)
{
	int i;
	int num;
	char *curr;

	i = 0;
	while (i < 3  && *rgb)
	{
		if ((num = ft_atoi(rgb)) > 255 || num < 0)
			break;
		if ((curr = skip_spaces(rgb)) == rgb && i == 0)
			break;
		rgb = curr;
		if ((curr = skip_digits(rgb)) == rgb)
			break;
		curr = skip_spaces(curr);
		if (i < 2 && *curr != ',')
			break;
		curr++;
		rgb = curr;
		*color = (*color << 8) + num;
		i++;
	}
	if (i < 3 || *rgb)
		return (KO);
	return (OK);
}

int	init_rgb(t_inf *inf, char *elem)
{
	int status;

	status = 0;
	if (elem[0] == 'F')
		status = add_rgb(&inf->color.floor, elem + 1);
	else if (elem[0] == 'C')
		status = add_rgb(&inf->color.ceil, elem + 1);
	if (status == KO)
		inf->message = COLOR_ERROR;
	return (status);
}

int	add_resltn(int *width, int *height, char *resltn)
{
	int i;
	int num;
	char *curr;

	i = 0;
	while (i < 2 && *resltn)
	{
		if ((num = ft_atoi(resltn)) < 0)
			break;
		if ((curr = skip_spaces(resltn)) == resltn)
			break;
		resltn = curr;
		if ((curr = skip_digits(resltn)) == resltn)
			break;
		resltn = curr;
		if (i == 0)
			*width = num > *width ? *width : num;
		else
			*height = num > *height ? *height : num;
		i++;
	}
	if (i < 2 || *resltn)
		return (KO);
	return (OK);
}

int	init_resltn(t_inf *inf, char *elem)
{
	int status;

	status = 0;
	if (elem[0] == 'R')
		status = add_resltn(&inf->width, &inf->height, elem + 1);
	if (status == KO)
		inf->message = RESOLUTION_ERROR;
	return (status);
}

int ismap(char *elem)
{
	while (*elem)
	{
		if (*elem != ' ' && *elem != '0' && *elem != '1' && *elem != '2')
			return (0);
		elem++;
	}
	return (1);
}

int	init_textr_rgb_resltn(char *elem, t_inf *inf)
{
	int status;

	if ((status = init_textr(inf, elem)) == OK)
		return (OK);
	else if (status != KO && (status = init_rgb(inf, elem)) == OK)
		return (OK);
	else if (status != KO && (status = init_resltn(inf, elem)) == OK)
		return (OK);
	else if (status != KO)
	{
		elem = skip_spaces(elem);
		if (*elem == '\0')
			status = 0;
		else if (ismap(elem))
		{
			status = KO;
			inf->message = NOT_ENOUGH_CONFIG;
		}
		else
		{
			status = KO;
			inf->message = UNKNOWN_ELEM;
		}
	}
	return (status);
}

int config_parser(int fd, t_inf *inf)
{
	int		num;
	char	*line;
	char	*elem;
	int		status;

	num = 0;
	status = 0;
	while (num < NUM_ELEMS && get_next_line(fd, &line) > 0 && status != KO)
	{
		if (!(elem = ft_strtrim(line, SPACES)))
		{
			status = KO;
			inf->message = NOT_ENOUGH_MEMORY;
		}
		else if ((status = init_textr_rgb_resltn(elem, inf)) == OK)
			num++;
		free_ptrs((void **)&elem, (void **)&line, (void **)0);
		inf->map_line++;
	}
	free_ptrs((void **)&elem, (void **)&line, (void **)0);
	if (status != KO && num < NUM_ELEMS)
	{
		status = KO;
		inf->message = NOT_ENOUGH_CONFIG;
	}
	return (status);
}

int map_config_parser(t_inf *inf, char *map_path)
{
    int		fd;
	int		status;

	if ((fd = open(map_path, O_RDONLY)) == KO)
		return (KO);
	if ((status = config_parser(fd, inf)) != KO)
		ft_putstr_fd("", 1);
		// status = map_parser(fd, inf);
	close(fd);
	return (status);
}

void print_error_message(int line, char *message)
{
	ft_putstr_fd("line ", 1);
	ft_putnbr_fd(line, 1);
	ft_putstr_fd(": ", 1);
	ft_putendl_fd(message, 1);
}

int main(int argc, char **argv)
{
    t_inf *inf;
	if (!(inf = (t_inf *)ft_calloc(1, sizeof(t_inf))))
		return (0);

    inf->mlx = mlx_init();

	mlx_get_screen_size(inf->mlx, &inf->width, &inf->height);

	if (map_config_parser(inf, "../map.cub") == KO)
	{
		print_error_message(inf->map_line, inf->message);
		close_game(inf);
	}

	ft_putnbr_fd(inf->width, 1);
	ft_putendl_fd("", 1);
	ft_putnbr_fd(inf->height, 1);
	ft_putendl_fd("", 1);
	
    inf->img.img_ptr = mlx_new_image(inf->mlx, inf->width, inf->height);
    inf->img.height = inf->height;
    inf->img.width = inf->width;
    inf->img.data = (int *)mlx_get_data_addr(inf->img.img_ptr,
                                        &inf->img.bpp,
                                        &inf->img.sl,
                                        &inf->img.endian);
    
    inf->color.ceil = 11656444;
    inf->color.floor = 8421504;

	inf->dist_buf = (double *)ft_calloc(inf->width, sizeof(double));

    int	map[MAP_HEIGHT][MAP_WIDTH] =
    {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,2,1,1,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,2,2,0,0,0,0,2,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,2,0,0,2,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,2,2,0,0,0,2,1,1,0,0,0,0,0,1,1,1,0,1,1,1},
        {1,0,0,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1},
        {1,1,1,1,0,1,1,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,0,1},
        {1,1,0,0,0,0,0,0,1,1,0,1,0,1,0,1,1,1,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,1,0,1},
        {1,1,0,0,0,0,0,0,1,1,0,1,0,1,0,1,1,1,1,1,0,1,1,1},
        {1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,2,2,2,0,2,2,1},
        {1,1,1,1,0,1,1,1,1,1,1,1,0,0,1,0,1,2,0,0,0,0,0,1},
        {1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,1,2,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,1,0,0,0,1,1},
        {1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,1,0,1,0,1},
        {1,1,0,0,0,0,0,1,1,1,0,0,0,1,1,0,1,0,1,0,0,0,1,1},
        {1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0,1,0,1,0,1,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0,1,0,1,0,1,0,1},
        {1,1,0,0,0,0,0,1,1,1,0,0,0,1,1,0,1,0,1,0,0,0,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };

    inf->map = fill_map(map);
    init_sprs(inf);
    
    t_view view;
    view.dir.x = 0.0;
    view.dir.y = 1.0;
    view.pos.x = 6.5;
    view.pos.y = 1.9;
    view.cam_plane.x = 0.66;
    view.cam_plane.y = 0;

    inf->view = view;

    inf->turn.cos_negative = cos(-TURN_SPEED);
    inf->turn.cos_pozitive = cos(TURN_SPEED);
    inf->turn.sin_negative = sin(-TURN_SPEED);
    inf->turn.sin_pozotive = sin(TURN_SPEED);

    inf->event.move_backword = 0;
    inf->event.move_forawrd = 0;
    inf->event.move_left = 0;
    inf->event.move_right = 0;
    inf->event.turn_left = 0;
    inf->event.turn_right = 0;

    if (argc != 1)
    {
		ft_putstr_fd(argv[0], 1);
        save_screen(inf);
        close_game(inf);        
    }

    inf->win = mlx_new_window(inf->mlx, inf->width, inf->height, "Test");

    mlx_hook(inf->win, 17, 1L << 17, close_game, inf);
	mlx_hook(inf->win, 2, 1L << 0, button_press, inf);
    mlx_loop_hook(inf->mlx, event_nadling, inf);
	ft_putendl_fd("1212", 1);
	mlx_hook(inf->win, 3, 1L << 1, button_release, inf);
	ft_putendl_fd("1214", 1);
	mlx_loop(inf->mlx);

    return 0;
}
// gcc -Werror -Wextra -Wall ../get_next_line/get_next_line.c ../get_next_line/get_next_line_utils.c test.c -L ../minilibx/ -lmlx -L ../libft/ -lft -lm -lX11 -lbsd -lXext -D BUFFER_SIZE=100
// gcc -Werror -Wextra -Wall test.c -L ../minilibx/ -lmlx -L ../libft/ -lft -lm -lX11 -lbsd -lXext